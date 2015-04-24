function result = testChange(Data)
Data(1:end,2)=0;
% Data(51:100,2)=100;
% Data(51:end-50,2)=100;
Data(51:65,2)=100;
Data(end-10:end,2)=100;
result=Data;