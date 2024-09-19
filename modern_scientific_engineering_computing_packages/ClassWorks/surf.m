x = -pi : 0.2: pi;
y = -pi : 0.2: pi;

[X, Y] = meshgrid(x, y);
Z = 20 - (X.^2) - (Y.^2);

surf(X,Y,Z);
