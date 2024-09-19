p = @(z) z.^3 - 1;
dp = @(z) 3*z.^2;
exact_roots = roots([1, 0, 0, -1]);
x_range = linspace(-2, 2, 500);
y_range = linspace(-2, 2, 500);
[X, Y] = meshgrid(x_range, y_range);
Z = X + 1i*Y;

for iter = 1:20
    Z = Z - p(Z)./dp(Z);
end

hold on;
colors = ['r', 'g', 'b'];

for i = 1:length(exact_roots)
    distances = abs(Z - exact_roots(i));
    is_close = distances < 0.0000001;
    plot(X(is_close), Y(is_close), '.', 'Color', colors(i), 'MarkerSize', 1);
end

% Настройка графического окна
xlabel('Re(z)');
ylabel('Im(z)');
hold off;
