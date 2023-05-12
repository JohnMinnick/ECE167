close all
clear all 

[rawAccel, rawMag, Adist, Bdist] = CreateTumbleData(1000);

AscaleFactor = 1 / 0.5*(2^15-1); % inverse of things from tumble func 
MscaleFactor = 0.15;

scale_Accel = rawAccel .* AscaleFactor; %scale
scale_Mag = rawMag .* MscaleFactor;

[Atilde_Accel,Btilde_Accel] = CalibrateEllipsoidData3D(scale_Accel(:,1),scale_Accel(:,2),scale_Accel(:,3),10,0); %calibration

[X_cor_Accel,Y_cor_Accel,Z_cor_Accel] = CorrectEllipsoidData3D(scale_Accel(:,1),scale_Accel(:,2),scale_Accel(:,3),Atilde_Accel,Btilde_Accel); %cal correction

norm_org_Accel = sqrt((scale_Accel(:,1) .^2)+(scale_Accel(:,2).^2)+(scale_Accel(:,3).^2)); %uncal norm
norm_calcor_Accel = sqrt((X_cor_Accel .^2)+(Y_cor_Accel .^2)+(Z_cor_Accel .^2)); % cal corrected norm 

figure(1)
histfit(norm_org_Accel);
title('Orginal Accelometer Data');
xlabel('Value (m/s)');
ylabel('# of Data Points');

figure(2)
histfit(norm_calcor_Accel);
title('Calibrated and Corrected Accelometer Data');
xlabel('Value (m/s)');
ylabel('# of Data Points');


[Atilde_Mag,Btilde_Mag] = CalibrateEllipsoidData3D(scale_Mag(:,1),scale_Mag(:,2),scale_Mag(:,3),10,0); %calibration

[X_cor_Mag,Y_cor_Mag,Z_cor_Mag] = CorrectEllipsoidData3D(scale_Mag(:,1),scale_Mag(:,2),scale_Mag(:,3),Atilde_Mag,Btilde_Mag); %cal correction

norm_org_Mag = sqrt((scale_Mag(:,1) .^2)+(scale_Mag(:,2).^2)+(scale_Mag(:,3).^2)); %uncal norm
norm_calcor_Mag = sqrt((X_cor_Mag .^2)+(Y_cor_Mag .^2)+(Z_cor_Mag .^2)); % cal corrected norm 
 
figure(3)
histfit(norm_org_Mag);
title('Orginal Magnetometer Data');
xlabel('Value (Unitized Magnetic Field)');
ylabel('# of Data Points');

figure(4)
histfit(norm_calcor_Mag);
title('Calibrated and Corrected Magnetometer Data');
xlabel('Value (Unitized Magnetic Field)');
ylabel('# of Data Points');


std_org_accel = std(norm_org_Accel)
mean_org_accel = mean(norm_org_Accel)
std_calcorr_accel = std(norm_calcor_Accel)
mean_calcorr_accel = mean(norm_calcor_Accel)

std_org_mag = std(norm_org_Mag)
mean_org_mag = mean(norm_org_Mag)
std_calcorr_mag = std(norm_calcor_Mag)
mean_calcorr_mag = mean(norm_calcor_Mag)
