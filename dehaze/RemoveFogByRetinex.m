function In = RemoveFogByRetinex( f, flag, outputfilename )
%RemoveFogByRetinex
%   Detailed explanation goes here
if nargin < 2
    flag = 1;
end

% RGB channels
fr = f(:, :, 1);
fg = f(:, :, 2);
fb = f(:, :, 3);

% type normalization
mr = mat2gray(im2double(fr));
mg = mat2gray(im2double(fg));
mb = mat2gray(im2double(fb));

% standard variance
alpha = 1458;

% size of template
n = 161;

% compute the center
n1 = floor((n + 1)/2);
for i = 1:n
    for j = 1:n
        % Gaussian function
        b(i, j) = exp(-((i - n1)^2 + (j - n1)^2)/(4*alpha))/(pi*alpha);
    end
end

% convolution filtering
nr1 = imfilter(mr, b, 'conv', 'replicate');
ng1 = imfilter(mg, b, 'conv', 'replicate');
nb1 = imfilter(mb, b, 'conv', 'replicate');
ur1 = log(nr1);
ug1 = log(ng1);
ub1 = log(nb1);
tr1 = log(mr);
tg1 = log(mg);
tb1 = log(mb);
yr1 = (tr1 - ur1)/3;
yg1 = (tg1 - ug1)/3;
yb1 = (tb1 - ub1)/3;

% standard variance
beta = 53.38;

% size of template
x = 31;

% compute the center
x1 = floor((n + 1)/2);
for i = 1:n
    for j = 1:n
        % Gaussian function
        a(i, j) = exp(-((i - n1)^2 + (j - n1)^2)/(4*beta))/(6*pi*beta);
    end
end

% convolution filtering
nr2 = imfilter(mr, a, 'conv', 'replicate');
ng2 = imfilter(mg, a, 'conv', 'replicate');
nb2 = imfilter(mb, a, 'conv', 'replicate');
ur2 = log(nr2);
ug2 = log(ng2);
ub2 = log(nb2);
tr2 = log(mr);
tg2 = log(mg);
tb2 = log(mb);
yr2 = (tr2 - ur2)/3;
yg2 = (tg2 - ug2)/3;
yb2 = (tb2 - ub2)/3;

% standard variance
eta = 13944.5;

% size of template
l = 501;

% compute the center
l1 = floor((n + 1)/2);
for i = 1:n
    for j = 1:n
        % Gaussian function
        e(i, j) = exp(-((i - n1)^2 + (j - n1)^2)/(4*eta))/(4*pi*eta);
    end
end
% convolution
nr3 = imfilter(mr, e, 'conv', 'replicate');
ng3 = imfilter(mg, e, 'conv', 'replicate');
nb3 = imfilter(mb, e, 'conv', 'replicate');
ur3 = log(nr3);
ug3 = log(ng3);
ub3 = log(nb3);
tr3 = log(mr);
tg3 = log(mg);
tb3 = log(mb);
yr3 = (tr3 - ur3)/3;
yg3 = (tg3 - ug3)/3;
yb3 = (tb3 - ub3)/3;

dr = yr1 + yr2 + yr3;
dg = yg1 + yg2 + yg3;
db = yb1 + yb2 + yb3;
cr = im2uint8(dr);
cg = im2uint8(dg);
cb = im2uint8(db);
In = cat(3, cr, cg, cb);

if flag
    figure;
    subplot(2, 2, 1); imshow(f); title('Original', 'FontWeight', 'Bold');
    subplot(2, 2, 2); imshow(In); title('Results', 'FontWeight', 'Bold'); imwrite(In, outputfilename);
    Q = rgb2gray(f);
    W = rgb2gray(In);
    subplot(2, 2, 3); imhist(Q, 64); title('Original Histogram', 'FontWeight', 'Bold');
    subplot(2, 2, 4); imhist(W, 64); title('Result Histogram', 'FontWeight', 'Bold');
end
end

