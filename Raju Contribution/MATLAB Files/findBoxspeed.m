function speed_box=findBoxspeed(test_obtData)
% It automatically calculates the box speed i.e target speed that lies in
% opto coverage.
%INPUT test_obtData  data obtained in matrix form
%OUTPUT speed_box  scalar variable equivalent to speed of box.

optoData= test_obtData; %#codegen

contOne=0;
temp=zeros(size(optoData));
for i=1:length(optoData)-1    
    if optoData(i)==100 && optoData(i+1)==100 %Continous check
        if i==length(optoData)-1 %if last element has 100           
            contOne=contOne+2;
            temp(i)=contOne;  
        else
            contOne=contOne+1;
        end
    end 
    if contOne>0 && optoData(i)==100 && optoData(i+1)==0 %Ending check
        contOne=contOne+1;
    end
    if contOne==0 && optoData(i)==100 && optoData(i+1)==0 %Just for single '1'
        contOne=contOne+1;
    end
    if contOne>0 && optoData(i)==0
              temp(i)=contOne;
        contOne=0;
    end
end
speed_box= max(temp);
end