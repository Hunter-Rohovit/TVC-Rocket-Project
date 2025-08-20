function q = eul_to_quat(roll, pitch, yaw)
    % Inputs in degrees
    %Converting to radians
    roll = deg2rad(roll);
    pitch = deg2rad(pitch);
    yaw = deg2rad(yaw);

    % Converts ZYX Euler angles (yaw, pitch, roll) to quaternion [w x y z]
    cy = cos(yaw/2);
    sy = sin(yaw/2);
    cp = cos(pitch/2);
    sp = sin(pitch/2);
    cr = cos(roll/2);
    sr = sin(roll/2);

    q0 = cr*cp*cy+sr*sp*sy;
    q1 = sr*cp*cy-cr*sp*sy;
    q2 = cr*sp*cy+sr*cp*sy;
    q3 = cr*cp*sy-sr*sp*cy;

    %Because AOA is defined from inertial Z-axis, we need to make an offset
    %rotation
    q_offset = [sqrt(2)/2; 0; -sqrt(2)/2; 0];
    q = quatmultiply(q_offset', [q0, q1, q2, q3]);
    q = q';
end