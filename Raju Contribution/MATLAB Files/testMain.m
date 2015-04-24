clc;
clear all;
close all;
%%%some parameters
speed_box=150; %Set for test purpose: The assumption is desired box is active in opto range for 3 second 
variation=50;
xl_name= 'test1.xls';
sheet_name='all the data';
%% Data Loading and formatting : making ready for analysis
load('DataMat1.mat');
obtData= DataMat(1:8001,:); %Taken around 320 data for analysis (just for example)
formatted_MAT = dataFormatting1(obtData); % Formatted 3D and opto sensor data, presented in 8 column format; [SN opto x1 x2 x3 y1 y2 y3]
test_obtData=testChange(formatted_MAT); %Change opto value just to make ease to change opto data only:: For real data analysis, we will not use this function
%% This can find the presence and absence of box and identifies the noise or
% any other objects different than our desired box
display('Checking real test data..........');
yORn=checkBox(test_obtData,speed_box,variation);

% Checking Spreadsheet data..........
display('Checking Spreadsheet data..........');
% [nm,s1,s2,s3,s4,nm_i,s4_i]=dataRead_spreadsheet(xl_name,sheet_name);
[nm,s1,s2,s3,s4,nm_i,s4_i]=dataRead_spreadsheet1(xl_name,sheet_name);
xl_Data_nm= nm;
yORn=checkBox(xl_Data_nm,speed_box,variation);

% Test metal presence
xl_Data_s1= s1;
xl_Data_s2= s2;
xl_Data_s3= s3;
xl_Data_s4= s4;
xl_Data_nm_i= nm_i;
xl_Data_s4_i= s4_i;
testPlot();

