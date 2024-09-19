x = linspace(0, 1, 1000);
y = humps(x);
plot(x, y);
hold on;

[y_max, idx_max] = max(y);
x_max = x(idx_max);

plot(x_max, y_max, 'r.', 'MarkerSize', 20);
idx_interval = y >= 20 & y <= 40;
x_interval = x(idx_interval);
y_interval = y(idx_interval);
plot(x_interval, y_interval, 'p')
