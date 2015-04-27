function cvVal = findCV (resultant)
% This function calculates the coefficient of variation (CV)
% INPUT resultant -> input vector
% OUTPUT cvVal -> scalar, coefficient of variation value

meanVal = mean(resultant);%#codegen
varVal = std(resultant(:));%standard deviation
cvVal=varVal./meanVal;%test_obtData