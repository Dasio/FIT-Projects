%% 1
img = imread('xmikus15.bmp');
H = ones(3,3) * -0.5;
H(2,2) = 5.0;
s1 = imfilter(img,H);
imwrite(s1,'step1.bmp');

%% 2
s2 = fliplr(s1);
imwrite(s2,'step2.bmp');

%% 3
s3 = medfilt2(s2, [5 5]);
imwrite(s3,'step3.bmp');

%% 4
H = ones(5,5);
H(2:4, 2:4) = 3;
H(3,3) = 9;
H = H/49;
s4 = imfilter(s3,H);
imwrite(s4,'step4.bmp');
%% Chyba
% Otocenie naspet
eimg = fliplr(s4);
resolution = size(eimg);
esum = 0.0;
for j = 1:resolution(1)
    for k = 1:resolution(2)
        esum = esum + abs(double(img(j,k)) - double(eimg(j,k)));
    end
end
chyba = esum / (resolution(1) * resolution(2))
%% 5
s5 = im2double(s4);
minVal = min(s5(:));
maxVal = max(s5(:));
% Posun dolava
s5 = s5 - minVal;
% O kolko sa ma vynasobit aby max bol 1.0
mul = 1 / (maxVal - minVal);
s5 = im2uint8(s5 * mul);
imwrite(s5,'step5.bmp');
%% reseni.txt
% s4 - pred, s5- po
s4 = im2double(s4);
s5 = im2double(s5);
mean_no_hist = mean2(s4) * 255.0
std_no_hist = std2(s4) * 255.0
mean_hist = mean2(s5) * 255.0
std_hist = std2(s5) * 255.0

%% 6
s6 = zeros(resolution);
a = 0.0;
b = 1.0;
N = 2;
for j = 1:resolution(1)
    for k = 1:resolution(2)
        s6(j,k) = round(((2^N)-1)*(s5(j,k)-a)/(b-a))*(b-a)/((2^N)-1) + a;
    end
end
imwrite(s6, 'step6.bmp');

