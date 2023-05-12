% John Minnick
% ECE167 
% 5/11/2023
% Lab 3
% Author's Note: Noise in CTD has been reduced but NOT removed 

%clear all 
close all

[Accel Mag Adist Bdist] = CreateTumbleData(1000);
Accel(:,1) = (Accel(:,1) ./ (range(Accel(:,1)) / 2));
Accel(:,2) = (Accel(:,2) ./ (range(Accel(:,2)) / 2));
Accel(:,3) = (Accel(:,3) ./ (range(Accel(:,3)) / 2));
meanA = mean(Accel);
Accel(:,1) = (Accel(:,1) - meanA(1,1));
Accel(:,2) = (Accel(:,2) - meanA(1,2));
Accel(:,3) = (Accel(:,3) - meanA(1,3));


figure(1)
scatter3(Accel(:,1),Accel(:,2),Accel(:,3));
axis equal;


Mag(:,1) = (Mag(:,1) ./ (range(Mag(:,1)) / 2));
Mag(:,2) = (Mag(:,2) ./ (range(Mag(:,2)) / 2));
Mag(:,3) = (Mag(:,3) ./ (range(Mag(:,3)) / 2));
meanM = mean(Mag);
Mag(:,1) = (Mag(:,1) - meanM(1,1));
Mag(:,2) = (Mag(:,2) - meanM(1,2));
Mag(:,3) = (Mag(:,3) - meanM(1,3));

figure(2)
scatter3(Mag(:,1),Mag(:,2),Mag(:,3));
axis equal;
%max_A = max(Accel);
%min_A = min(Accel);
%mean_A = mean(Accel);

norm_cal_accel = sqrt(Accel(:,1).^2 + Accel(:,2).^2 + Accel(:,3).^2);
figure(3)
histfit(norm_cal_accel);
std(norm_cal_accel)

norm_cal_mag = sqrt(Mag(:,1).^2 + Mag(:,2).^2 + Mag(:,3).^2);
figure(4)
histfit(norm_cal_mag);
std(norm_cal_mag)