f = @(t) sin(t);
x = -4*pi:pi/100:4*pi;
y = f(x);

figure
plot(x, y)
hold on
axis equal

plot_array = []
for i = 1:5
  plot_array(i) = plot(0, 0);
endfor

hold off

r = 1/2;
n = 100;
phi = 0:pi/100:2*pi;
u = @(phi) r .* cos(phi);
v = @(phi) r .* sin(phi);
x0 = -4*pi;

for i = 1:n
  xt = x0;
  for k = 1:5
    yt = f(xt);
    set(plot_array(k), 'XData', u(phi) + xt, 'YData', v(phi) + yt);
    xt += 1.5 * r;
  endfor
  x0 += r/4;
  drawnow
  pause(0.05)
endfor
