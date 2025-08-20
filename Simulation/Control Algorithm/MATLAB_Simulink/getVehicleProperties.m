function [mass, CG, I, Cd_A, Cl_A, Cy_A, Cn_A, Cm_A, CP, C_damp] = getVehicleProperties(t, alpha, beta, alpha_total,vehicle, aeroData)    
    %Get rid of any small floating digits
    alpha(abs(alpha) < 1e-6) = 0;
    beta(abs(beta) < 1e-6) = 0;

    %tau is the normalized time (between 0-1) based on burn time
    %min,max ensures the mass stays within it's real range
    tau = min(max(t/vehicle.burnTime,0),1);

    %Calculate new mass, CG, I
    mass = (1-tau)*vehicle.mass0 +tau*vehicle.massF;
    CG = (1-tau)*vehicle.CG0 +tau*vehicle.CGF;
    I = (1-tau)*vehicle.I0 + tau*vehicle.IF;

    %Get Aerodynamic Properties based on angle of attack
    Cd_A = interp1(aeroData.AOA, aeroData.CdA, alpha_total, 'linear', 'extrap');
    Cl_A = interp1(aeroData.AOA, aeroData.ClA,  alpha, 'linear', 'extrap');
    Cy_A = interp1(aeroData.AOA, aeroData.ClA, beta, 'linear', 'extrap');
    Cn_A = interp1(aeroData.AOA, aeroData.CnA,  alpha_total, 'linear', 'extrap');
    Cm_A = interp1(aeroData.AOA, aeroData.CmA,  alpha_total, 'linear', 'extrap');
    CP   = interp1(aeroData.AOA, aeroData.CP,  alpha_total, 'linear', 'extrap');
    C_damp = interp1(aeroData.AOA, aeroData.damp, alpha_total, 'linear','extrap');

end
