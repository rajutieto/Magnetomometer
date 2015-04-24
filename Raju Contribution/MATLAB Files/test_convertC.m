clc;
clear all;
close all;
%%%some parameters
ref_speed_box=150; %Set for test purpose: The assumption is desired box is active in opto range for 3 second 
variation_box=50;
ref_cv=0.1;
variation_cv=0.01;
max_cv=0.8;
xl_name= 'test1.xls';
sheet_name='all the data';
% sheet_name='fast';
% roller_type = 'always';
roller_type = 'conditional'; % 'always' is for always spinning roller &
                        %'conditional'is for roller that rotates only in box presence
                        %'unknown' is for not to consider roller effect

%% Checking Spreadsheet data..........
% display('Checking Spreadsheet data..........');
test1=xlsread(xl_name,sheet_name); % Read numeric data from 'all the data' sheet of 'test1.xls file'
test_obtData=dataRead(test1);
% resultant = find_resultant(test_obtData);
speed_box=findBoxspeed(test_obtData(:,2));
box_stat=checkBox(ref_speed_box,speed_box,variation_box);

% Test metal presence
if box_stat==1 %Box presence
    resultant = find_resultant(test_obtData);
    cvVal=findCV(resultant);
%     roller_noise = calcRollerEffect(test_obtData,roller_type,cvVal);
    load('roller_cond.mat');
    k=size(test_obtData,1);
    roller_resultantAdjust=adjustRollerData(roller_resultant,k);
    roller_removed = filterRollerEffect(resultant,roller_resultantAdjust,roller_type);
    metal_stat= checkMetal(cvVal,ref_cv,variation_cv);
    largenoisemetal_stat=checkLargeNoiseMetal(cvVal,max_cv);
    displayStatus(box_stat,speed_box,cvVal,metal_stat,largenoisemetal_stat,roller_type);
%     plot_filtering(resultant,roller_noise,metal_stat);
end
plot(1:size(resultant,1),[resultant roller_removed])
legend('Roller Affected','Roller Noise Filtered')
title('Roller Effect Filtering');

% testPlot();

