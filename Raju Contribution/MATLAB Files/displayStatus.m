function displayStatus(obj_stat,speed_box,cvVal,metal_stat,largenoisemetal_stat,roller_type)

%Display object status
if obj_stat==1; %BOX present
    disp('BOX PRESENT');
end
if obj_stat==-1; % Nothing present
    disp('NO BOX PRESENT');
end
if obj_stat==0; %Noise present
    disp('Other Noise PRESENT');
end

%Measure box speed..
disp(['The box speed is   = ' num2str(speed_box)]);

%Dispaly cv Value
disp(['The cv value of resultant data is  = ' num2str(cvVal)]);

%Display metal status
if metal_stat==1; %BOX present
    disp('CLEAN BOX-- NO METAL PRESENT');
end
if metal_stat==0; %Noise present
    disp('CONTAMINATED BOX--METAL DETECTED');
end

%Display large metal around status
if largenoisemetal_stat==1; %Noise present
    disp('LARGE UNEXPECTED METAL AROUND DETECTED');
end
if largenoisemetal_stat==-1; %Noise present
    disp('LARGE METAL DETECTED (Expected inside box)');
end
if largenoisemetal_stat==0; %Noise present
    disp('NO LARGE METAL AROUND DETECTED');
end

%Display roller type
if strcmp(roller_type,'conditional')==1
    disp('Roller-----Spins with box only')
end
if strcmp(roller_type,'always')==1
    disp('Roller-----Always Spinning')
end



