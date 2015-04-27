function roller_noise = calcRollerEffect(test_obtData,roller_type,cvVal)

roller_type= 'conditional'; %Test Purpose
% temp_data = [test_obtData(:,2),resultant];
roller_noise =zeros(size(test_obtData,1),1);
speed_box=findBoxspeed(test_obtData);
optoOntime=speed_box;
factor1=1;
factor2=1;

if strcmp(roller_type,'conditional')==1 %Our test Roller
    startOpto = strfind((test_obtData(:,2))', ones(1,optoOntime)*100);
    startOpto = min(startOpto);
    endOpto=startOpto+optoOntime-1;
    roller_noise(1:startOpto-1,1)= sort(rand(startOpto-1,1),'ascend');
    roller_noise(startOpto:endOpto,1)= roller_noise(startOpto-1,1)+ rand(speed_box,1)*cvVal;
    roller_noise(endOpto+1:size(test_obtData,1))= (roller_noise(endOpto,1)-sort(rand(size(test_obtData,1)-endOpto,1),'ascend')*0.7);    
     
elseif strcmp(roller_type,'always')==1 %Labbats Roller
%     roller_noise=0;
    roller_noise= rand(size(test_obtData,1),1);
    
elseif strcmp(roller_type,'unknown')==1 % No Roller Effect
    roller_noise=0;
end

