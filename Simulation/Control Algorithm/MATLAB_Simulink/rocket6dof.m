function xdot = rocket6dof(t,x,u,vehicle, env, aeroData, thrustTime, thrustData)
    %get values from state vector
    q = x(1:4); %quaternion components, [q0;q1;q2;q3]
    omega = x(5:7); %angular_velocity [p, q, r]
    pos = x(8:10); %position [x,y,z]
    vel = x(11:13); %velocity [Vx,Vy,Vz]

    %Quaternion kinematics matrix
    % Uses the quaternion derivative to evaluate how the quaternion changes
    % over time with angular velocity
    Omega = 0.5 * [0, -omega(1), -omega(2), -omega(3);
                  omega(1), 0, omega(3), -omega(2);
                  omega(2), -omega(3), 0, omega(1);
                  omega(3), omega(2), -omega(1), 0];

    % Compute Quaternion derivative from angular velocity
    q_dot = Omega*q;


    %create rotation matrices for transformation between vehicle 
    %and inertial reference frames 
    R_body_inertial = quat2rotm(q');
    R_inertial_body = R_body_inertial';

    %determine velocity
    v_rel_inertial = vel-env.wind(:); %velocity relative to air
    v_body = R_inertial_body * v_rel_inertial; %convert to vehicle reference frame
    v_body_mag = norm(v_body); %magnitude of free stream velocity
    if v_body_mag < 1e-6
        v_body_dir = [1; 0; 0];  % Default direction when at rest
    else
        v_body_dir = v_body / v_body_mag;
    end
    display(v_body);
    %compute total angle of attack (includes both pitch and side slip
    %angles)
    cos_theta = dot([1;0;0], v_body) / norm(v_body);
    cos_theta = max(min(cos_theta, 1), -1);  % clamp for safety
    alpha_total = acos(cos_theta); 
    alpha_total = rad2deg(alpha_total);

    alpha = atan2(v_body(3), v_body(1));  % signed AOA (Z component vs X)
    alpha = rad2deg(alpha);
    beta  = atan2(v_body(2), v_body(1));
    beta = rad2deg(beta);


    %get mass & aerodynamic properties for vehicle
    [mass, CG, I, Cd_A, Cl_A, Cy_A, ~, Cm_A, CP, C_damp] = getVehicleProperties(t, alpha, beta, alpha_total, vehicle, aeroData); 
    
    display(alpha_total);
    display(alpha);
    display(mass);
    display(CG);
    display(I);
    display(Cd_A);
    display(Cl_A);
    display(beta);
    display(Cy_A);
    display(Cm_A);
    display(CP);
    display(C_damp);


    %Compute Aerodynamic Forces
    q = 0.5*env.rho*v_body_mag^2; %dynamic pressure

    %drag is always in the opposite direction of velocity, paralell to the
    %rocket's longitudinal axis
    F_drag = -q*Cd_A*v_body_dir;

    %Lift is perpendicular to the velocity vector, however because this is
    %a 3d space there's an entire plane perpendicular to the velocity
    %vector. If we use a reference axis (the rocket's Z axis) we can define
    %a vector for lift
    z_body = [0;0;1]; 
    %double cross product to project z axis into the plane perpendicular to
    %the velocity vector
    lift_dir = cross(cross(v_body_dir,z_body), v_body_dir);
    lift_norm = norm(lift_dir);
    if lift_norm < 1e-6
        lift_dir = [0; 0; 0];
    else
        lift_dir = lift_dir / lift_norm;
    end
    
    F_lift = q * Cl_A * lift_dir;

    %compute the side slip force
    slip_dir = cross(-v_body_dir, lift_dir);
    slip_norm = norm(slip_dir);

    if slip_norm < 1e-6
        slip_dir = [0; 0; 0];
    else
        slip_dir = slip_dir / slip_norm;
    end

    F_slip = q * Cy_A * slip_dir;

    display(F_drag);
    display('F_Drag_Inertial');
    display(R_body_inertial*F_drag);
    display(F_lift);
    display('F_lift_Inertial');
    display(R_body_inertial*F_lift);
    display(F_slip);
    display('F_Slip_Inertial');
    display(R_body_inertial*F_slip);

    %Summing the forces
    F_aero_body = F_drag + F_lift + F_slip; 

    %Find force from thrust vectoring
    thrust_angle_pitch = deg2rad(u(1));
    thrust_angle_yaw = deg2rad(u(2));
    thrust_dir_body = [cos(thrust_angle_pitch)*cos(thrust_angle_yaw); sin(thrust_angle_yaw); sin(thrust_angle_pitch)];
    thrust_dir_body = thrust_dir_body/norm(thrust_dir_body);
    thrust = interp1(thrustTime(:,1), thrustData(:,1), t, 'linear', 0);
    F_thrust = thrust*thrust_dir_body;
    F_thrust_inertial = R_body_inertial * F_thrust;

    display('Gimbal Angles: (pitch,yaw)');
    display(u(1));
    display(u(2));
    display(F_thrust);
    display(F_thrust_inertial);

    %Gravity Force
    F_gravity = [0;0;-env.g*mass];

    Forces_Body_Sum = F_aero_body + F_thrust; %Sum of forces in body frame of reference
    Forces_Inertial_Sum = R_body_inertial*Forces_Body_Sum + F_gravity; %Convert to inertial reference frame

    display(Forces_Body_Sum);
    display(Forces_Inertial_Sum);

    %Compute Aerodynamic Moment
    display(CG);
    display(CP);
    r_aero = [CG-CP;0;0]; %lever arm goes from CG to CP (+bodyX direction)
    M_aero_body = cross(r_aero, F_aero_body);

    display(r_aero);
    display(F_aero_body);
    display(M_aero_body);

    %calculate the moments due to thrust vectoring
    %Lever arm is along the longitudinal axis of the rocket
    leverArm_pitch = [CG-vehicle.servoPitchPivotDistance; 0; 0];
    leverArm_yaw = [CG-vehicle.servoYawPivotDistance; 0; 0];
    %moment = rxF
    M_pitch_thrust = cross(leverArm_pitch, F_thrust);
    M_yaw_thrust = cross(leverArm_yaw, F_thrust);
    M_thrust_body = M_yaw_thrust + M_pitch_thrust;

    %compute Aerodynamic Damping Moment
    C2A = C_damp*v_body_mag;
    M_damp = [0;-C2A*omega(2);-C2A*omega(3)];
    display(M_damp);

    %Sum of moments
    Moments_Body_Sum = M_aero_body + M_thrust_body +M_damp;

    %Find the Translational Acceleration
    translational_accel = Forces_Inertial_Sum/mass;

    %Find Angular Acceleration using Euler's Equations
    angular_accel = I\(Moments_Body_Sum-cross(omega, (I*omega)));
    
    %final state derivative vector:
    xdot = [q_dot(:);
            angular_accel(:);
            vel(:);
            translational_accel(:)];

    disp("Wind:"); disp(env.wind);
    disp("vel_rel_inertial"); disp(v_rel_inertial);
    disp("alpha (deg):"); disp(alpha);
    disp("F_drag_inertial:"); disp(R_body_inertial*F_drag);
    disp("M_aero:"); disp(M_aero_body);



    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
% Visualiztion of Vectors in a 3D space

% Given/known quantities at current timestep
q = x(1:4);  % [phi, theta, psi]
pos = x(8:10);  % position of rocket in inertial frame    
origin = pos(:);  % rocket location in inertial space

% Rocket body axes in body frame
X_b = [1; 0; 0];  % forward
Y_b = [0; 1; 0];  % right
Z_b = [0; 0; 1];  % down (typical for body frame)

% Transform to inertial frame
R = quat2rotm(q');
X_i = R * X_b;
Y_i = R * Y_b;
Z_i = R * Z_b;

% Transform aerodynamic force vectors to inertial
F_drag_i  = R * F_drag;
F_lift_i  = R * F_lift;
F_slip_i  = R * F_slip;

% Add thrust and gravity (already in inertial frame)
F_thrust_i = F_thrust_inertial;
F_gravity_i = [0; 0; -mass * env.g];

% Normalize for plotting
scale = 0.2;
F_drag_i_plot    = scale * F_drag_i    / norm(F_drag_i);
F_lift_i_plot    = scale * F_lift_i    / norm(F_lift_i);
F_slip_i_plot    = scale * F_slip_i    / (norm(F_slip_i) + 1e-6);
F_thrust_i_plot  = scale * F_thrust_i  / (norm(F_thrust_i) + 1e-6);
F_gravity_i_plot = scale * F_gravity_i / norm(F_gravity_i);

% Plotting
figure;
hold on;
axis equal;
grid on;
xlabel('X');
ylabel('Y');
zlabel('Z');
title('Rocket Force Vectors and Body Axes');

% Inertial frame axes
quiver3(origin(1), origin(2), origin(3), 1, 0, 0, 0.3, 'r', 'LineWidth', 1.5); % X
quiver3(origin(1), origin(2), origin(3), 0, 1, 0, 0.3, 'g', 'LineWidth', 1.5); % Y
quiver3(origin(1), origin(2), origin(3), 0, 0, 1, 0.3, 'b', 'LineWidth', 1.5); % Z

% Body frame axes
quiver3(origin(1), origin(2), origin(3), X_i(1), X_i(2), X_i(3), 0.5, 'k',   'LineWidth', 2);
quiver3(origin(1), origin(2), origin(3), Y_i(1), Y_i(2), Y_i(3), 0.5, 'k--', 'LineWidth', 2);
quiver3(origin(1), origin(2), origin(3), Z_i(1), Z_i(2), Z_i(3), 0.5, 'k:',  'LineWidth', 2);

% Forces
quiver3(origin(1), origin(2), origin(3), F_drag_i_plot(1),    F_drag_i_plot(2),    F_drag_i_plot(3),    1, 'c',  'LineWidth', 2);
quiver3(origin(1), origin(2), origin(3), F_lift_i_plot(1),    F_lift_i_plot(2),    F_lift_i_plot(3),    1, 'c--',  'LineWidth', 2);
quiver3(origin(1), origin(2), origin(3), F_slip_i_plot(1),    F_slip_i_plot(2),    F_slip_i_plot(3),    1, 'c:',  'LineWidth', 2);
quiver3(origin(1), origin(2), origin(3), F_thrust_i_plot(1),  F_thrust_i_plot(2),  F_thrust_i_plot(3),  1, 'm',  'LineWidth', 2);
%quiver3(origin(1), origin(2), origin(3), F_gravity_i_plot(1), F_gravity_i_plot(2), F_gravity_i_plot(3), 1, 'y',  'LineWidth', 2);

%%%Now do Moments
M_aero_inertial  = R_body_inertial * M_aero_body;
M_thrust_inertial = R_body_inertial * M_thrust_body;

%Normalize for plotting
M_scale = 0.2;
M_aero_plot   = M_scale * M_aero_inertial   / (norm(M_aero_inertial)   + 1e-6);
M_thrust_plot = M_scale * M_thrust_inertial / (norm(M_thrust_inertial) + 1e-6);

% Plot Moment vectors
quiver3(origin(1), origin(2), origin(3), M_thrust_plot(1), M_thrust_plot(2), M_thrust_plot(3), 1, 'm--', 'LineWidth', 2);
quiver3(origin(1), origin(2), origin(3), M_aero_plot(1),   M_aero_plot(2),   M_aero_plot(3),   1, 'c', 'LineWidth', 2); 

legend({'Inertial X','Inertial Y','Inertial Z', ...
        'Body X','Body Y','Body Z', ...
        'Drag','Lift','Slip','Thrust', ...
        'Thrust Moment','Aero Moment'});

view(3);
display(M_aero_plot);
end
