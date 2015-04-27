function All_data = dataFormatting1(DataMat)
% clc;
% clear all;
% close all;
% load('C:\Users\ASADULLAH\Raju Dropbox\Dropbox\oDesk\interview\Ongoing\metal detector\Matlab\test_feb03\DataMat1.mat');
if length(DataMat)<26
    All_data=[0,0,0,0,0,0,0,0];
else
    Data1=DataMat(2:end,:);
    len1=length(Data1);
    % len1=6026
    quot_Mat1=floor(len1/25);
    len2=quot_Mat1*25;
    Data2=Data1(1:len2,:);

    x1_1=Data2(1:25:end,:);
    x1_2=Data2(2:25:end,:);
    x1_3=Data2(3:25:end,:);
    x1_4=Data2(4:25:end,:);

    y1_1=Data2(5:25:end,:);
    y1_2=Data2(6:25:end,:);
    y1_3=Data2(7:25:end,:);
    y1_4=Data2(8:25:end,:);

    z1_1=Data2(9:25:end,:);
    z1_2=Data2(10:25:end,:);
    z1_3=Data2(11:25:end,:);
    z1_4=Data2(12:25:end,:);

    x2_1=Data2(13:25:end,:);
    x2_2=Data2(14:25:end,:);
    x2_3=Data2(15:25:end,:);
    x2_4=Data2(16:25:end,:);

    y2_1=Data2(17:25:end,:);
    y2_2=Data2(18:25:end,:);
    y2_3=Data2(19:25:end,:);
    y2_4=Data2(20:25:end,:);

    z2_1=Data2(21:25:end,:);
    z2_2=Data2(22:25:end,:);
    z2_3=Data2(23:25:end,:);
    z2_4=Data2(24:25:end,:);

    opto=Data2(25:25:end,:);

    h_x1_1=char(dec2hex(x1_1));
    h_x1_2=char(dec2hex(x1_2));
    h_x1_3=char(dec2hex(x1_3));
    h_x1_4=char(dec2hex(x1_4));

    h_y1_1=char(dec2hex(y1_1));
    h_y1_2=char(dec2hex(y1_2));
    h_y1_3=char(dec2hex(y1_3));
    h_y1_4=char(dec2hex(y1_4));

    h_z1_1=char(dec2hex(z1_1));
    h_z1_2=char(dec2hex(z1_2));
    h_z1_3=char(dec2hex(z1_3));
    h_z1_4=char(dec2hex(z1_4));

    h_x2_1=char(dec2hex(x2_1));
    h_x2_2=char(dec2hex(x2_2));
    h_x2_3=char(dec2hex(x2_3));
    h_x2_4=char(dec2hex(x2_4));

    h_y2_1=char(dec2hex(y2_1));
    h_y2_2=char(dec2hex(y2_2));
    h_y2_3=char(dec2hex(y2_3));
    h_y2_4=char(dec2hex(y2_4));

    h_z2_1=char(dec2hex(z2_1));
    h_z2_2=char(dec2hex(z2_2));
    h_z2_3=char(dec2hex(z2_3));
    h_z2_4=char(dec2hex(z2_4));

    hex_x1=strcat(h_x1_4,h_x1_3,h_x1_2,h_x1_1);
    hex_y1=strcat(h_y1_4,h_y1_3,h_y1_2,h_y1_1);
    hex_z1=strcat(h_z1_4,h_z1_3,h_z1_2,h_z1_1);

    hex_x2=strcat(h_x2_4,h_x2_3,h_x2_2,h_x2_1);
    hex_y2=strcat(h_y2_4,h_y2_3,h_y2_2,h_y2_1);
    hex_z2=strcat(h_z2_4,h_z2_3,h_z2_2,h_z2_1);

    dec_x1=double(typecast( uint32( hex2dec(hex_x1) ), 'int32'));
    dec_y1=double(typecast( uint32( hex2dec(hex_y1) ), 'int32'));
    dec_z1=double(typecast( uint32( hex2dec(hex_z1) ), 'int32'));
    dec_x2=double(typecast( uint32( hex2dec(hex_x2) ), 'int32'));
    dec_y2=double(typecast( uint32( hex2dec(hex_y2) ), 'int32'));
    dec_z2=double(typecast( uint32( hex2dec(hex_z2) ), 'int32'));

    sensor1= [dec_x1,dec_y1,dec_z1];
    sensor2= [dec_x2,dec_y2,dec_z2];

    SN=[1:1:quot_Mat1].';

    All_data=[SN,opto,sensor1,sensor2];
end