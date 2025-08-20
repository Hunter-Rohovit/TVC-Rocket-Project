data = readmatrix('ThrustCurve.xlsx');
time = data(:,1);
thrust = data(:,2);
thrustCurve = timeseries(thrust,time);