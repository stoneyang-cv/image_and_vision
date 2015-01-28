function In = RemoveFogByGlobalHisteq( I, flag )
%RemoveFogByGlobalHisteq
%   Detailed explanation goes here

if nargin < 2
    flag = 1;
end
R = I(:,:,1);
G = I(:,:,2);
B = I(:,:,3);
M = histeq(R);
N = histeq(G);
L = histeq(B);
In = cat(3, M, N, L);
if flag
    figure;
    subplot(2, 2, 1); imshow(I); title('Original', 'FontWeight', 'Bold');
    subplot(2, 2, 2); imshow(In); title('Results', 'FontWeight', 'Bold');
    Q = rgb2gray(I);
    W = rgb2gray(In);
    subplot(2, 2, 3); imhist(Q, 64); title('Original histogram', 'FontWeight', 'Bold');
    subplot(2, 2, 4); imhist(W, 64); title('Results histogram', 'FontWeight', 'Bold');
end
end

