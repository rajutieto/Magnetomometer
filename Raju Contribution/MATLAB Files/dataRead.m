function [test_obtData] = dataRead(test1)
%dataRead(test1) is a function that abstarcts one test data from whole excel sheet.
%INPUT test1-- whole excel unformatted data
%test_obtData---formatted 8 column matrix, 1st column time, 2nd column opto
%,3rd column x1 data, 4th column y1 data, 5th column z1, 6th column x2 data
%7th column y2 data and 8th column z2 data

%- NaN is replaced with 0
test1(isnan(test1))=0; %#codegen
test1_data= test1(2:239,2:end); 
% test1_data= test1(125:126,2:end);
opto_data=test1_data(:,2);
x1_data=test1_data(:,3);
y1_data=test1_data(:,4);
z1_data=test1_data(:,5);
x2_data=test1_data(:,6);
y2_data=test1_data(:,7);
z2_data=test1_data(:,8);
time_data= length (opto_data);
test_obtData=zeros(size(test1_data));
test_obtData(:,1)=1:time_data;
test_obtData(:,2)=opto_data;
test_obtData(:,3)=x1_data;
test_obtData(:,4)=y1_data;
test_obtData(:,5)=z1_data;
test_obtData(:,6)=x2_data;
test_obtData(:,7)=y2_data;
test_obtData(:,8)=z2_data;

end