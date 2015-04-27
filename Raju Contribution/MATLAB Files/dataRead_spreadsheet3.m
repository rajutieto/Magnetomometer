function [nm,s1,s2,s3,s4,nm_i,s4_i] = dataRead_spreadsheet3(xl_name,sheet_name)

%----------------- Begin code  -------------
test1=xlsread (xl_name,sheet_name); % Read numeric data from 'all the data' sheet of 'test1.xls file'
% test1=xlsread ('test1.xls','all the data');
test1(isnan(test1))=0; % NaN is replaced with 0

% This sample code compares and plots six sensor data with no metal and sample1
% metal for a single test. This can be extended for number of tests taking
% average data (or any appropriate mathematical) for each sensor.

%%No metal case
% _nm is used to denote no metal data matrix
% test1_nm= abs(test1(2:239,2:end));
% test1_nm= test1(2:239,2:end);
% test1_nm= test1(241:474,2:end);
% test1_nm= test1(476:717,2:end);
test1_nm= test1(718:963,2:end);
opto_nm=test1_nm(:,2);
x1_nm=test1_nm(:,3);
y1_nm=test1_nm(:,4);
z1_nm=test1_nm(:,5);
x2_nm=test1_nm(:,6);
y2_nm=test1_nm(:,7);
z2_nm=test1_nm(:,8);
time_nm= length (opto_nm);
test_obtData_nm(:,1)=1:time_nm;
test_obtData_nm(:,2)=opto_nm;
test_obtData_nm(:,3)=x1_nm;
test_obtData_nm(:,4)=y1_nm;
test_obtData_nm(:,5)=z1_nm;
test_obtData_nm(:,6)=x2_nm;
test_obtData_nm(:,7)=y2_nm;
test_obtData_nm(:,8)=z2_nm;

%%sample metal1 case
% _s1 is used to denote sample1 metal data matrix

% test1_s1= abs(test1(1443:1666,2:end));
% test1_s1= test1(1443:1666,2:end);
% test1_s1= test1(1668:1921,2:end);
% test1_s1= test1(1923:2145,2:end);
test1_s1= test1(2146:2419,2:end);
opto_s1=test1_s1(:,2);
x1_s1=test1_s1(:,3);
y1_s1=test1_s1(:,4);
z1_s1=test1_s1(:,5);
x2_s1=test1_s1(:,6);
y2_s1=test1_s1(:,7);
z2_s1=test1_s1(:,8);
time_s1= length (opto_s1);
test_obtData_s1(:,1)=1:time_s1;
test_obtData_s1(:,2)=opto_s1;
test_obtData_s1(:,3)=x1_s1;
test_obtData_s1(:,4)=y1_s1;
test_obtData_s1(:,5)=z1_s1;
test_obtData_s1(:,6)=x2_s1;
test_obtData_s1(:,7)=y2_s1;
test_obtData_s1(:,8)=z2_s1;

%%sample metal2 case
% test1_s2= abs(test1(3393:3621,2:end));
% test1_s2= test1(3393:3621,2:end);
% test1_s2= test1(3622:3876,2:end);
% test1_s2= test1(3877:4103,2:end);
test1_s2= test1(4104:4369,2:end);
opto_s2=test1_s2(:,2);
x1_s2=test1_s2(:,3);
y1_s2=test1_s2(:,4);
z1_s2=test1_s2(:,5);
x2_s2=test1_s2(:,6);
y2_s2=test1_s2(:,7);
z2_s2=test1_s2(:,8);
time_s2= length (opto_s2);
test_obtData_s2(:,1)=1:time_s2;
test_obtData_s2(:,2)=opto_s2;
test_obtData_s2(:,3)=x1_s2;
test_obtData_s2(:,4)=y1_s2;
test_obtData_s2(:,5)=z1_s2;
test_obtData_s2(:,6)=x2_s2;
test_obtData_s2(:,7)=y2_s2;
test_obtData_s2(:,8)=z2_s2;

%%sample metal3 case
% test1_s3= abs(test1(5360:5596,2:end));
% test1_s3= test1(5360:5596,2:end);
% test1_s3= test1(5597:5861,2:end);
% test1_s3= test1(5862:6096,2:end);
test1_s3= test1(6097:6365,2:end);
opto_s3=test1_s3(:,2);
x1_s3=test1_s3(:,3);
y1_s3=test1_s3(:,4);
z1_s3=test1_s3(:,5);
x2_s3=test1_s3(:,6);
y2_s3=test1_s3(:,7);
z2_s3=test1_s3(:,8);
time_s3= length (opto_s3);
test_obtData_s3(:,1)=1:time_s3;
test_obtData_s3(:,2)=opto_s3;
test_obtData_s3(:,3)=x1_s3;
test_obtData_s3(:,4)=y1_s3;
test_obtData_s3(:,5)=z1_s3;
test_obtData_s3(:,6)=x2_s3;
test_obtData_s3(:,7)=y2_s3;
test_obtData_s3(:,8)=z2_s3;
%%sample metal4 case
% test1_s4= abs(test1(7424:7658,2:end));
% test1_s4= test1(7424:7658,2:end);
% test1_s4= test1(7659:7919,2:end);
% test1_s4= test1(7920:8149,2:end);
test1_s4= test1(8150:8397,2:end);
opto_s4=test1_s4(:,2);
x1_s4=test1_s4(:,3);
y1_s4=test1_s4(:,4);
z1_s4=test1_s4(:,5);
x2_s4=test1_s4(:,6);
y2_s4=test1_s4(:,7);
z2_s4=test1_s4(:,8);
time_s4= length (opto_s4);
test_obtData_s4(:,1)=1:time_s4;
test_obtData_s4(:,2)=opto_s4;
test_obtData_s4(:,3)=x1_s4;
test_obtData_s4(:,4)=y1_s4;
test_obtData_s4(:,5)=z1_s4;
test_obtData_s4(:,6)=x2_s4;
test_obtData_s4(:,7)=y2_s4;
test_obtData_s4(:,8)=z2_s4;

%%No metal with interference case
% test1_nm_i= abs(test1(9355:9586,2:end));
% test1_nm_i= test1(9355:9586,2:end);
% test1_nm_i= test1(9587:9838,2:end);
% test1_nm_i= test1(9839:10064,2:end);
test1_nm_i= test1(10065:10312,2:end);
opto_nm_i=test1_nm_i(:,2);
x1_nm_i=test1_nm_i(:,3);
y1_nm_i=test1_nm_i(:,4);
z1_nm_i=test1_nm_i(:,5);
x2_nm_i=test1_nm_i(:,6);
y2_nm_i=test1_nm_i(:,7);
z2_nm_i=test1_nm_i(:,8);
time_nm_i= length (opto_nm_i);
test_obtData_nm_i(:,1)=1:time_nm_i;
test_obtData_nm_i(:,2)=opto_nm_i;
test_obtData_nm_i(:,3)=x1_nm_i;
test_obtData_nm_i(:,4)=y1_nm_i;
test_obtData_nm_i(:,5)=z1_nm_i;
test_obtData_nm_i(:,6)=x2_nm_i;
test_obtData_nm_i(:,7)=y2_nm_i;
test_obtData_nm_i(:,8)=z2_nm_i;
%%sample metal4 with interference case
% test1_s4_i= abs(test1(10313:10543,2:end));
% test1_s4_i= test1(10313:10543,2:end);
% test1_s4_i= test1(10544:10800,2:end);
% test1_s4_i= test1(10801:11028,2:end);
test1_s4_i= test1(11029:11287,2:end);
opto_s4_i=test1_s4_i(:,2);
x1_s4_i=test1_s4_i(:,3);
y1_s4_i=test1_s4_i(:,4);
z1_s4_i=test1_s4_i(:,5);
x2_s4_i=test1_s4_i(:,6);
y2_s4_i=test1_s4_i(:,7);
z2_s4_i=test1_s4_i(:,8);
time_s4_i= length (opto_s4_i);
test_obtData_s4_i(:,1)=1:time_s4_i;
test_obtData_s4_i(:,2)=opto_s4_i;
test_obtData_s4_i(:,3)=x1_s4_i;
test_obtData_s4_i(:,4)=y1_s4_i;
test_obtData_s4_i(:,5)=z1_s4_i;
test_obtData_s4_i(:,6)=x2_s4_i;
test_obtData_s4_i(:,7)=y2_s4_i;
test_obtData_s4_i(:,8)=z2_s4_i;

nm=test_obtData_nm;
s1=test_obtData_s1;
s2=test_obtData_s2;
s3=test_obtData_s3;
s4=test_obtData_s4;
nm_i=test_obtData_nm_i;
s4_i=test_obtData_s4_i;

% size(test_obtData_nm);
% test_obtData(:,:,1)=test_obtData_nm;
% test_obtData(:,:,2)=test_obtData_s1;
% test_obtData(:,:,3)=test_obtData_s2;
% test_obtData(:,:,4)=test_obtData_s3;
% test_obtData(:,:,5)=test_obtData_s4;
% test_obtData(:,:,6)=test_obtData_nm_i;
% test_obtData(:,:,7)=test_obtData_s4_i;



% time_array= [time_nm, time_s1, time_s2, time_s3, time_s4, time_nm_i, time_s4_i];
% time= 1:min(time_array);
% time_length= length(time);
% 
% % For sensor x1
% % subplot(3,2,1)
% figure;
% plot(time,x1_nm(1:time_length,:),time,x1_s1(1:time_length,:),time,x1_s2(1:time_length,:),time,x1_s3(1:time_length,:),time,x1_s4(1:time_length,:),time,x1_nm_i(1:time_length,:),time,x1_s4_i(1:time_length,:));
% legend('No Metal','sample1','sample2','sample3','sample4','No Metal\_inter','sample4\_inter');
% xlabel('Time');
% ylabel('Magnetic Field');
% title('Magnetic field variation in sensor x1')
% 
% % For sensor y1
% % subplot(3,2,2)
% figure;
% plot(time,y1_nm(1:time_length,:),time,y1_s1(1:time_length,:),time,y1_s2(1:time_length,:),time,y1_s3(1:time_length,:),time,y1_s4(1:time_length,:),time,y1_nm_i(1:time_length,:),time,y1_s4_i(1:time_length,:));
% legend('No Metal','sample1','sample2','sample3','sample4','No Metal\_inter','sample4\_inter');
% xlabel('Time');
% ylabel('Magnetic Field');
% title('Magnetic field variation in sensor y1')
% 
% % For sensor z1
% % subplot(3,2,3)
% figure;
% plot(time,z1_nm(1:time_length,:),time,z1_s1(1:time_length,:),time,z1_s2(1:time_length,:),time,z1_s3(1:time_length,:),time,z1_s4(1:time_length,:),time,z1_nm_i(1:time_length,:),time,z1_s4_i(1:time_length,:));
% legend('No Metal','sample1','sample2','sample3','sample4','No Metal\_inter','sample4\_inter');
% xlabel('Time');
% ylabel('Magnetic Field');
% title('Magnetic field variation in sensor z1')
% 
% % For sensor x2
% % subplot(3,2,4)
% figure;
% plot(time,x2_nm(1:time_length,:),time,x2_s1(1:time_length,:),time,x2_s2(1:time_length,:),time,x2_s3(1:time_length,:),time,x2_s4(1:time_length,:),time,x2_nm_i(1:time_length,:),time,x2_s4_i(1:time_length,:));
% legend('No Metal','sample1','sample2','sample3','sample4','No Metal\_inter','sample4\_inter');
% xlabel('Time');
% ylabel('Magnetic Field');
% title('Magnetic field variation in sensor x2')
% 
% % For sensor y2
% % subplot(3,2,5)
% figure;
% plot(time,y2_nm(1:time_length,:),time,y2_s1(1:time_length,:),time,y2_s2(1:time_length,:),time,y2_s3(1:time_length,:),time,y2_s4(1:time_length,:),time,x2_nm_i(1:time_length,:),time,y2_s4_i(1:time_length,:));
% legend('No Metal','sample1','sample2','sample3','sample4','No Metal\_inter','sample4\_inter');
% xlabel('Time');
% ylabel('Magnetic Field');
% title('Magnetic field variation in sensor y2')
% 
% % For sensor z2
% % subplot(3,2,6)
% figure;
% plot(time,z2_nm(1:time_length,:),time,z2_s1(1:time_length,:),time,z2_s2(1:time_length,:),time,z2_s3(1:time_length,:),time,z2_s4(1:time_length,:),time,z2_nm_i(1:time_length,:),time,z2_s4_i(1:time_length,:));
% legend('No Metal','sample1','sample2','sample3','sample4','No Metal\_inter','sample4\_inter');
% xlabel('Time');
% ylabel('Magnetic Field');
% title('Magnetic field variation in sensor z2')
% % 
% % %---------------- End of code -----------------------------
% % figure;
% X1=[x1_nm;x1_nm_i;x1_s1;x1_s2;x1_s3;x1_s4;x1_s4_i];
% OPTO= [opto_nm;opto_nm_i;opto_s1;opto_s2;opto_s3;opto_s4;opto_s4_i];
% t=[1:length(OPTO)]';
% % plot(t,X1,t,OPTO);
% % title('For Sensor X1');
% % hold on;
% % figure;
% X2=[x2_nm;x2_nm_i;x2_s1;x2_s2;x2_s3;x2_s4;x2_s4_i];
% % plot(t,X2,t,OPTO);
% % title('For Sensor X2');
% % hold on;
% % figure;
% Y1=[y1_nm;y1_nm_i;y1_s1;y1_s2;y1_s3;y1_s4;y1_s4_i];
% % plot(t,Y1,t,OPTO);
% % title('For Sensor Y1');
% % hold on;
% % figure;
% Y2=[y2_nm;y2_nm_i;y2_s1;y2_s2;y2_s3;y2_s4;y2_s4_i];
% % plot(t,Y2,t,OPTO);
% % title('For Sensor Y2');
% % hold on;
% % figure;
% Z1=[z1_nm;z1_nm_i;z1_s1;z1_s2;z1_s3;z1_s4;z1_s4_i];
% % plot(t,Z1,t,OPTO);
% % title('For Sensor Z1');
% 
% % figure;
% 
% Z2=[z2_nm;z2_nm_i;z2_s1;z2_s2;z2_s3;z2_s4;z2_s4_i];
% % figure;
% subplot(2,1,1);
% plot(t,X1,t,X2,t,Y1,t,Y2,t,Z1,t,Z2,t,OPTO);
% legend('X1','X2','Y1','Y2','Z1','Z2','OPTO');
% % figure;
% subplot(2,1,2);
% plot(t,X1.^2,t,X2.^2,t,Y1.^2,t,Y2.^2,t,Z1.^2,t,Z2.^2,t,100.*OPTO.^2);
% legend('X1^2','X2^2','Y1^2','Y2^2','Z1^2','Z2^2','OPTO');
% % title('For Sensor Z2');
% figure;
% 
% SumAll_1=X1.^2+Y1.^2+Z1.^2;
% SumAll_2=X2.^2+Y2.^2+Z2.^2;
% SumAll=SumAll_1+SumAll_2;
% 
% plot(t,SumAll_1,t,SumAll_2,t,100.*OPTO.^2);
% legend('First','Second');
% 
% % Y_nm=sqrt(x1_nm.^2+y1_nm.^2+z1_nm.^2);
% % Y1=sqrt(x2_s1.^2+y2_s1.^2+z2_s1.^2);
% % Y2=sqrt(x2_s2.^2+y2_s2.^2+z2_s2.^2);
% % Y3=sqrt(x2_s3.^2+y2_s3.^2+z2_s3.^2);
% % Y4=sqrt(x2_s4.^2+y2_s4.^2+z2_s4.^2);
% % Y4_i=sqrt(x2_s4_i.^2+y2_s4_i.^2+z2_s4_i.^2);
% % Y=[Y_nm;Y1;Y2;Y3;Y4;Y4_i];
% % opto= [opto_nm;opto_s1;opto_s2;opto_s3;opto_s4;opto_s4_i].*10;
% % t=[1:length(opto)]';
% % plot(t,Y,t,opto);
% % axis([-5 1500 -5 3000])
% % hold on;
% % plot(t,opto_nm.*7);