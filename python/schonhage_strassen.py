def schonhage_strassen_multiply(a, b):
    # Convert integers to polynomials
    # For simplicity, this example treats each digit as a separate coefficient
    poly_a = [int(digit) for digit in str(a)]
    poly_b = [int(digit) for digit in str(b)]

    # Pad polynomials so their length is a power of 2
    n = 1
    while n < len(poly_a) + len(poly_b) - 1:
        n *= 2
    poly_a.extend([0] * (n - len(poly_a)))
    poly_b.extend([0] * (n - len(poly_b)))

    # Compute FFT of both polynomials
    fft_a = np.fft.fft(poly_a)
    print(fft_a)
    fft_b = np.fft.fft(poly_b)
    

    # Point-wise multiplication
    fft_product = fft_a * fft_b

    # Inverse FFT
    product_poly = np.fft.ifft(fft_product)

    # Convert polynomial back to integer
    # This part should handle carry, but is omitted for simplicity
    result = sum([int(round(coeff)) * (10 ** i) for i, coeff in enumerate(product_poly)])

    return result

# Example usage

if __name__ == '__main__':
    result = schonhage_strassen_multiply(123456789, 987654321)
    print(result)
    truth = 123456789 * 987654321
    print(truth)
