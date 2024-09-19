t = linspace(-5, 5, 500);
x = cos(t) - 1/4 * cos(t * 8) + 1/16 * sin(8 * t)
y = sin(t) + 1/4 * sin(t * 8) + 1/16 * cos(8 * t)
  
k = linspace(1,100,100);

hold on;

for i = 1:10
  plot(x/i, y/i);
endfor
