function optoON_array= findmaxOne(optoData)

contOne=0;
temp=0;
for i=1:length(optoData)-1
    if optoData(i)==100 && optoData(i+1)==100 %Conttinous check
        if i==length(optoData)-1 %if last element has 100
            contOne=contOne+2;
            temp=[temp;contOne];            
        else
            contOne=contOne+1;
        end
    end 
    if contOne>0 && optoData(i)==100 && optoData(i+1)==0 %Ending check
        contOne=contOne+1;
    end
    if contOne==0 && optoData(i)==100 && optoData(i+1)==0 %Just for single '1'
        contOne=contOne+1:
    end
    if contOne>0 && optoData(i)==0
            temp=[temp;contOne];
        contOne=0;
    end
end
disp(temp);
optoON_array=temp;