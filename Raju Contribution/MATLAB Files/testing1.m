% clc;
% close all;
% clear all; 
% t = tcpip('192.168.0.112',1200);
% set(t, 'InputBufferSize', 30000); 
% fopen(t); 
% disp(t);
% FirstConnection= fscanf(t)
% pause(1)
% fprintf(t,'send');
% DataMat=[0];
% disp(t);
% % test=0;
% tStart=tic;
% while length(DataMat)>5000
%     while t.BytesAvailable == 0
%         pause(0.01)
%     end
%     if get(t,'BytesAvailable') > 1
%         Data= fread(t,t.BytesAvailable);
%     end
%     DataMat=cat(1,DataMat,Data);
%     formattedData=dataFormatting (DataMat);
%     
%     
%     
%     fprintf(t,'send');
%     disp(t);    
% end
    
% fclose(t); 
% delete(t); 
% clear t 
clc;
clear all;
close all;
speed_box=200;
HappyD=zeros(1,8);
% load('C:\Users\ASADULLAH\Raju Dropbox\Dropbox\oDesk\interview\Ongoing\metal detector\Matlab\test_feb03\DataMat.mat');
load('C:\Users\ASADULLAH\Raju Dropbox\Dropbox\oDesk\interview\Ongoing\metal detector\Matlab\Feb09\DataMat1.mat')
obtData= DataMat(1:8001,:);
formatted_MAT = dataFormatting1(obtData);
test_obtData=testChange(formatted_MAT);
% obtData1=DataMat(2002:3001,:);
% optoONtime= countOne(obtData1);
yORn=checkBox(test_obtData,speed_box);

if yORn==1
    disp('BOX PRESENT');
elseif yORn==0
    disp('Short Noise PRESENT');
elseif yORn==-1
    disp('NO BOX PRESENT');
end

% Happy1=dataFormatting1(obtData1);
% Happy1=[Happy;Happy1];
