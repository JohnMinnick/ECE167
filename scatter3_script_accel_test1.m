Accel_XYZ_Sychronous_data;
X_uncal = Accel_XYZ_Syncro(:,1);
Y_uncal = Accel_XYZ_Syncro(:,2);
Z_uncal = Accel_XYZ_Syncro(:,3);
figure(1);
scatter3(X_uncal,Y_uncal,Z_uncal)

X_cal = 0.0010 * X_uncal + 0.0031;
Y_cal = 0.0010 * Y_uncal + 0.052;
Z_cal = 0.0010 * Z_uncal + 0.0109;
figure(2);
scatter3(X_cal,Y_cal,Z_cal)

 figure(3)
 plot([sqrt(X_uncal.^2+Y_uncal.^2+Z_uncal.^2) sqrt(X_cal.^2+Y_cal.^2+Y_cal.^2)],'.');
    legend('Pre-Calibration','Post-Calibration');
    title('Norm of data pre- and post-Calibration');