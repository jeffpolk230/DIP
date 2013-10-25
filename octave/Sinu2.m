
clear all;

function showImage(x)
	figure(1);
	clf
	imshow(x,[]);
	colormap(gray);
	return;
end 

function showBoth(origin, transform)
	figure(1);
	clf
	imshow(origin,[]);
	figure(2);
	clf
	imshow(transform,[]);
	colormap(gray);
	return;
end

function transform = ftPlusShift (origin)
	transform = fftshift (abs (fft2(origin)));
	return;
end


%%%% Normalize number, take origin min(a), origin max(b) %%%%
%%%% new min(x), new Max(y) and origin number "in" %%%%
function out  = normalization(a, b, x, y, in)
	out =  (in - a) / (b - a) * (y - x) + x;
	return;
end

%%%% Env Vars %%%%
nPix = 200;
mat = zeros(nPix,nPix);
f = 1; % frequency


for i = 1:nPix
	for j=1:nPix
		mat(i,j) = normalization (-1, 1, 0, 255, sin(2 * pi * f * j/nPix));
	endfor
endfor
% plot(mat(1,:)) % Should give a sinusoid bounded by 0 - 255

F = fft2(mat);
showBoth(mat, fftshift(abs(F)));

% Rotating the image by 90 degree
mat2 = rot90(mat, -1);
F2 = fft2(mat2);


boxMat = zeros(nPix,nPix);
boxSize = 30;

for i = (nPix - boxSize)/2:(nPix + boxSize)/2
	for j = (nPix - boxSize)/2:(nPix + boxSize)/2
		boxMat(i,j) = 1;
	endfor
endfor
