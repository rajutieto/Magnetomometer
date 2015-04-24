function count= countOne (Data)

optoData= Data(:,2);
count=length(optoData(optoData==100));