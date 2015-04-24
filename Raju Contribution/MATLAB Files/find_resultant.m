function resultant = find_resultant(test_obtData)
% This function is to calcutate resultant of 6 sensor data. We have used
% square root of x1, y1 and z1 and similarly for x2, y2 and z2. and finally
% taking square root of two obtained result.
% INPUT test_obtData -> matrix containing all data
% OUTPUT resultant -> 1 column resultant vector

data_x1= test_obtData(:,3);%#codegen
data_y1= test_obtData(:,4);
data_z1= test_obtData(:,5);
data_x2= test_obtData(:,6);
data_y2= test_obtData(:,7);
data_z2= test_obtData(:,8);

S1_nm= (data_x1.^2+data_y1.^2+data_z1.^2).^(0.5);
S2_nm= (data_x2.^2+data_y2.^2+data_z2.^2).^(0.5);
resultant=(S1_nm.^2+S2_nm.^2).^(0.5);