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

    %Gravity Force
    F_gravity = [0;0;-env.g*mass];

    Forces_Body_Sum = F_aero_body + F_thrust; %Sum of forces in body frame of reference
    Forces_Inertial_Sum = R_body_inertial*Forces_Body_Sum + F_gravity; %Convert to inertial reference frame

    %Compute Aerodynamic Moment
    r_aero = [CG-CP;0;0]; %lever arm goes from CG to CP (+bodyX direction)
    M_aero_body = cross(r_aero, F_aero_body);

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

end
