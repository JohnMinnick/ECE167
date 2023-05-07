% Sample uncalibrated and calibrated data as column vectors
uncalibrated_data = Z_Accel_Up;
calibrated_data = 0.0010 * Z_Accel_Up + 0.0109;

% Plot the point clouds
figure;
hold on;
scatter(1:numel(uncalibrated_data), uncalibrated_data, 'r', 'filled');
scatter(1:numel(calibrated_data), calibrated_data, 'b', 'filled');
xlabel('Index'); ylabel('Value');
legend('Uncalibrated', 'Calibrated');
title('Point Cloud of Uncalibrated and Calibrated Data');
hold off;
