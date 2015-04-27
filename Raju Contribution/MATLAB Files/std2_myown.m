function s = std2_myown(a)

fast_data_type = isa(a,'logical') || isa(a,'int8') || isa(a,'uint8') || ...
    isa(a,'uint16') || isa(a,'int16');

% only use IMHIST for images of sufficient size
big_enough = numel(a) > 300000;

if fast_data_type && isequal(ndims(a),2) && ~issparse(a) && big_enough
    
    % compute histogram
    if islogical(a)
        num_bins = 2;
    else
        data_type = class(a);
        num_bins = double(intmax(data_type)) - double(intmin(data_type)) + 1;
    end
    [bin_counts bin_values] = imhist(a, num_bins);

    % compute standard deviation
    total_pixels = numel(a);
    
    sum_of_pixels = sum(bin_counts .* bin_values);
    mean_pixel = sum_of_pixels / total_pixels;
    
    bin_value_offsets      = bin_values - mean_pixel;
    bin_value_offsets_sqrd = bin_value_offsets .^ 2;
    
    offset_summation = sum( bin_counts .* bin_value_offsets_sqrd);
    s = sqrt(offset_summation / total_pixels);
    
else
    
    % use simple implementation
    if ~isa(a,'double')
        a = double(a);
    end
    s = std(a(:));
    
end





