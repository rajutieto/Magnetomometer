
% [nm,s1,s2,s3,s4,nm_i,s4_i]=dataRead_Excel(xl_name,sheet_name);

nm_x1= nm(:,3);
nm_y1= nm(:,4);
nm_z1= nm(:,5);
nm_x2= nm(:,6);
nm_y2= nm(:,7);
nm_z2= nm(:,8);

s1_x1= s1(:,3);
s1_y1= s1(:,4);
s1_z1= s1(:,5);
s1_x2= s1(:,6);
s1_y2= s1(:,7);
s1_z2= s1(:,8);

s2_x1= s2(:,3);
s2_y1= s2(:,4);
s2_z1= s2(:,5);
s2_x2= s2(:,6);
s2_y2= s2(:,7);
s2_z2= s2(:,8);

s3_x1= s3(:,3);
s3_y1= s3(:,4);
s3_z1= s3(:,5);
s3_x2= s3(:,6);
s3_y2= s3(:,7);
s3_z2= s3(:,8);

s4_x1= s4(:,3);
s4_y1= s4(:,4);
s4_z1= s4(:,5);
s4_x2= s4(:,6);
s4_y2= s4(:,7);
s4_z2= s4(:,8);

nm_i_x1= nm_i(:,3);
nm_i_y1= nm_i(:,4);
nm_i_z1= nm_i(:,5);
nm_i_x2= nm_i(:,6);
nm_i_y2= nm_i(:,7);
nm_i_z2= nm_i(:,8);

s4_i_x1= s4_i(:,3);
s4_i_y1= s4_i(:,4);
s4_i_z1= s4_i(:,5);
s4_i_x2= s4_i(:,6);
s4_i_y2= s4_i(:,7);
s4_i_z2= s4_i(:,8);

S1_nm= (nm_x1.^2+nm_y1.^2+nm_z1.^2).^(0.5);
S2_nm= (nm_x2.^2+nm_y2.^2+nm_z2.^2).^(0.5);
NM_all=(S1_nm.^2+S2_nm.^2).^(0.5);

S1_s1= (s1_x1.^2+s1_y1.^2+s1_z1.^2).^(0.5);
S2_s1= (s1_x2.^2+s1_y2.^2+s1_z2.^2).^(0.5);
S1_all=(S1_s1.^2+S2_s1.^2).^(0.5);

S1_s2= (s2_x1.^2+s2_y1.^2+s2_z1.^2).^(0.5);
S2_s2= (s2_x2.^2+s2_y2.^2+s2_z2.^2).^(0.5);
S2_all=(S1_s2.^2+S2_s2.^2).^(0.5);

S1_s3= (s3_x1.^2+s3_y1.^2+s3_z1.^2).^(0.5);
S2_s3= (s3_x2.^2+s3_y2.^2+s3_z2.^2).^(0.5);
S3_all=(S1_s3.^2+S2_s3.^2).^(0.5);

S1_s4= (s4_x1.^2+s4_y1.^2+s4_z1.^2).^(0.5);
S2_s4= (s4_x2.^2+s4_y2.^2+s4_z2.^2).^(0.5);
S4_all=(S1_s4.^2+S2_s4.^2).^(0.5);

S1_nm_i= (nm_i_x1.^2+nm_i_y1.^2+nm_i_z1.^2).^(0.5);
S2_nm_i= (nm_i_x2.^2+nm_i_y2.^2+nm_i_z2.^2).^(0.5);
NM_i_all=(S1_nm_i.^2+S2_nm_i.^2).^(0.5);

S1_s4_i= (s4_i_x1.^2+s4_i_y1.^2+s4_i_z1.^2).^(0.5);
S2_s4_i= (s4_i_x2.^2+s4_i_y2.^2+s4_i_z2.^2).^(0.5);
S4_i_all=(S1_s4_i.^2+S2_s4_i.^2).^(0.5);











time_nm=size(nm,1);
time_s1=size(s1,1);
time_s2=size(s2,1);
time_s3=size(s3,1);
time_s4=size(s4,1);
time_nm_i=size(nm_i,1);
time_s4_i=size(s4_i,1);

time_array= [time_nm, time_s1, time_s2, time_s3, time_s4, time_nm_i, time_s4_i];
time= 1:min(time_array);
time_length= length(time);

% For sensor S1
subplot(2,1,1)
% figure;
plot(time,S1_nm(1:time_length,:),time,S1_s1(1:time_length,:),time,S1_s2(1:time_length,:),time,S1_s3(1:time_length,:),time,S1_s4(1:time_length,:),time,S1_nm_i(1:time_length,:),time,S1_s4_i(1:time_length,:));
legend('No Metal','sample1','sample2','sample3','sample4','No Metal\_inter','sample4\_inter');
xlabel('Time');
ylabel('Magnetic Field');
title('Magnetic field variation in sensor in side 1')

% For sensor y1
subplot(2,1,2)
% figure;
plot(time,S2_nm(1:time_length,:),time,S2_s1(1:time_length,:),time,S2_s2(1:time_length,:),time,S2_s3(1:time_length,:),time,S2_s4(1:time_length,:),time,S2_nm_i(1:time_length,:),time,S2_s4_i(1:time_length,:));
legend('No Metal','sample1','sample2','sample3','sample4','No Metal\_inter','sample4\_inter');
xlabel('Time');
ylabel('Magnetic Field');
title('Magnetic field variation in sensor in side 2')

figure;
plot(time,NM_all(1:time_length,:),time,S1_all(1:time_length,:),time,S2_all(1:time_length,:),time,S3_all(1:time_length,:),time,S4_all(1:time_length,:),time,NM_i_all(1:time_length,:),time,S4_i_all(1:time_length,:));
legend('No Metal','sample1','sample2','sample3','sample4','No Metal\_inter','sample4\_inter');
% 
