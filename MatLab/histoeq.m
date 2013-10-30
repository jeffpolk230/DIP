%performing histogram equalization without using histeq
% takes input grayscale image ( M by N by 1)
% Jeff


function output = histoeq(img_path)

img = imread(img_path);
if size(img, 3) > 1 
    img = rgb2gray(img);
end
% figure, imshow(img);
% title('origin image[grayscaled]');


%construct histogram by hand
%finding frequency fo each value
freq = zeros(256, 1);
for i = 1:size(img,1)
    for j = 1:size(img,2)
        val = img(i,j);
        freq(val+1) = freq(val+1) +1;
    end
end 

figure, bar((1:256), freq); % this should be histogram :)

% Probability of occurrence as probf
probf = freq(:)/numel(img);
% sum(probf) % should be one

cum = zeros(256,1);
for i=1:size(probf)
    if i == 1
       cum(i) = cum(i) + freq(i); 
    else
        cum(i) =  cum(i-1) + freq(i);
    end
end
probc = cum(:)/numel(img);
o = round(probc(:)*255);

output=uint8(zeros(size(img,1), size(img,2)));

for i=1:size(img,1)
    for j=1:size(img,2)
        output(i,j) = o(img(i,j) + 1);
        val = output(i,j);
        freq(val+1) = freq(val+1) +1;
    end
end

figure, imshow(output);
title('histogram equalized output');
figure, bar((1:256), freq);
title('histogram after equlization');

end