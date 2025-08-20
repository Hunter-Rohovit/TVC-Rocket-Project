function [roll, pitch, yaw] = quat_to_eul(q)
    % Input quaternion q = [q0; q1; q2; q3] (as column vector)
    % where q0 is the scalar part

    % Undo the offset applied in eul_to_quat
    q_offset = [sqrt(2)/2; 0; -sqrt(2)/2; 0];
    q = quatmultiply(quatconj(q_offset') , q'); % remove offset

    q0 = q(1); q1 = q(2); q2 = q(3); q3 = q(4);

    % ZYX rotation order
    % Compute roll (x-axis rotation)
    sinr_cosp = 2 * (q0*q1 + q2*q3);
    cosr_cosp = 1 - 2 * (q1^2 + q2^2);
    roll = atan2(sinr_cosp, cosr_cosp);

    % Compute pitch (y-axis rotation)
    sinp = 2 * (q0*q2 - q3*q1);
    if abs(sinp) >= 1
        pitch = sign(sinp) * pi/2; % use 90 degrees if out of range
    else
        pitch = asin(sinp);
    end

    % Compute yaw (z-axis rotation)
    siny_cosp = 2 * (q0*q3 + q1*q2);
    cosy_cosp = 1 - 2 * (q2^2 + q3^2);
    yaw = atan2(siny_cosp, cosy_cosp);

    % Convert to degrees
    roll = rad2deg(roll);
    pitch = rad2deg(pitch)+90;
    yaw = rad2deg(yaw);
end