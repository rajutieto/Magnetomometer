rlr=roller_noise*400;
t=[1:size(rlr,1)]';
caps=resultant-rlr;
plot(t,resultant,t,rlr,t,rlr1)
grid on;
% plot(t,rlr)
legend('Total Magnetic Effect','Roller Effect Conditional','Roller Effect Always' );
xlabel('Box movement time')
ylabel('Resultant magnetic strength')
title('Test simulation for roller effect---The box has no metal')