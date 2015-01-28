function In = RemoveFogByLocalHisteq(I, flag)

g1 = GetLocalHisteq(I(:, :, 1));
g2 = GetLocalHisteq(I(:, :, 2));
g3 = GetLocalHisteq(I(:, :, 3));

In = cat(3, g1, g2, g3);

if flag
    figure;
    subplot(2, 2, 1); imshow(I); title('Original', 'FontWeight', 'Bold');
    subplot(2, 2, 2); imshow(In); title('Results', 'FontWeight', 'Bold');
    Q = rgb2gray(I);
    W = rgb2gray(In);
    subplot(2, 2, 3); imhist(Q, 64); title('Original Histogram', 'FontWeight', 'Bold');
    subplot(2, 2, 4); imhist(W, 64); title('Results Histogram', 'FontWeight', 'Bold');
end
    function g = GetLocalHisteq(I)
        x = mat2gray(I);
        f = im2double(x);
        w = 4;
        k = 0.06;
        M = mean2(f);
        z = colfilt(f, [w, w], 'sliding', @std);
        m = colfilt(f, [w, w], 'sliding', @mean);
        A = k * M./ z;
        g = A.* (f - m) + m;
        g = im2uint8(mat2gray(g));
    end
end