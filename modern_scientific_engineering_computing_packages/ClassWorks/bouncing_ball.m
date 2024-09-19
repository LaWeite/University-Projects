gravity_factor = 4;
g = 9.81 * gravity_factor;
h0 = 10;
y0 = [h0; 0];
time_range_end = 10;
time_range = [0, time_range_end];
epsilon = 1e-6;
trajectory = [];
max_iterations = 10;
f = @(t, y) [y(2); -g]; % y(1) = y, y(2) = y'

function [value, isterminal, direction] = event_touch_ground(t, y)
  value = y(1);    
  isterminal = 1;   
  direction = -1;   
end

for iteration = 1:max_iterations
  options = odeset('Events', @event_touch_ground);
  [t, y, te, ye, ie] = ode45(f, time_range, y0, options);
  trajectory = [trajectory; t, y];

  if isempty(ie)
    break;
  else
    velocity_of_event = ye(2);
    y0 = [0; -0.9 * velocity_of_event];
    time_range = [te, time_range_end];
  end

  if abs(diff(time_range)) < epsilon
    break;
  end
end

trajectory_time = trajectory(:,1);
trajectory_y = trajectory(:,2);
plot(trajectory_time, trajectory_y);
