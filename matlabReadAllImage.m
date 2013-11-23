fnames = dir ('*.jpg');
vals = cell(1, length(fnames));
for x = 1:length(fnames)
  vals{x} = imread(fname(x).name);
end
//READ'EM ALL!
