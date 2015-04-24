function plot_filtering(resultant,roller_noise,metal_stat)

plot(resultant);
hold on;
plot(roller_noise);
plot(resultant-roller_noise);