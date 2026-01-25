"""
@file decoder.py
@author Dejana Smiljanic
@date 17.01.2026.
@brief JPEG-like image reconstruction and visualization script.

This script reconstructs a grayscale image from RLE-compressed DCT coefficients
generated on a DSP. It performs inverse zig-zag scanning, dequantization,
inverse DCT, and visualizes the reconstructed image alongside the original
grayscale and RGB images, including an error (difference) map.
"""

import numpy as np
import matplotlib.pyplot as plt
from scipy.fftpack import idct
from PIL import Image
import os

## Image width in pixels
IMG_WIDTH = 256 

## Image height in pixels  
IMG_HEIGHT = 256 

## Size of one DCT block   
BLOCK_SIZE = 8

BLOCKS_X = IMG_WIDTH // BLOCK_SIZE    
BLOCKS_Y = IMG_HEIGHT // BLOCK_SIZE   
TOTAL_BLOCKS = BLOCKS_X * BLOCKS_Y 

print(f"=== KONFIGURACIJA: {IMG_WIDTH}x{IMG_HEIGHT} ===")
print(f"Blokova: {BLOCKS_X}x{BLOCKS_Y} = {TOTAL_BLOCKS}\n")

os.makedirs("results", exist_ok=True)

## Zig-zag index table for an 8x8 block
#
# Defines the scanning order used in JPEG compression
zigzag_8x8 = np.array([
    0,  1,  5,  6,  14, 15, 27, 28,
    2,  4,  7,  13, 16, 26, 29, 42,
    3,  8,  12, 17, 25, 30, 41, 43,
    9,  11, 18, 24, 31, 40, 44, 53,
    10, 19, 23, 32, 39, 45, 52, 54,
    20, 22, 33, 38, 46, 51, 55, 60,
    21, 34, 37, 47, 50, 56, 59, 61,
    35, 36, 48, 49, 57, 58, 62, 63
])

## JPEG standard quantization table
#
# Used for dequantization during image reconstruction
Q_TABLE = np.array([
    16, 11, 10, 16, 24, 40, 51, 61,
    12, 12, 14, 19, 26, 58, 60, 55,
    14, 13, 16, 24, 40, 57, 69, 56,
    14, 17, 22, 29, 51, 87, 80, 62,
    18, 22, 37, 56, 68, 109, 103, 77,
    24, 35, 55, 64, 81, 104, 113, 92,
    49, 64, 78, 87, 103, 121, 120, 101,
    72, 92, 95, 98, 112, 100, 103, 99
], dtype=np.float32).reshape(8, 8)


def inverse_zigzag_8x8(zz_coeffs):
    """
    @brief Converts zig-zag ordered coefficients back to an 8x8 block.

    @param zz_coeffs 1D array of 64 zig-zag ordered coefficients
    @return 8x8 numpy array representing the frequency block
    """
    block = np.zeros(64, dtype=np.float32)
    for i in range(64):
        block[zigzag_8x8[i]] = float(zz_coeffs[i])
    return block.reshape(8, 8)

def idct2_block(block):
    """
    @brief Performs 2D inverse DCT on an 8x8 block.

    @param block 8x8 array of frequency-domain coefficients
    @return 8x8 array in spatial domain
    """
    return idct(idct(block.T, norm='ortho').T, norm='ortho')


def decode_rle_block(dc_diff, ac_list, prev_dc):
    """
    @brief Performs 2D inverse DCT on an 8x8 block.

    @param block 8x8 array of frequency-domain coefficients
    @return 8x8 array in spatial domain
    """
    zz = np.zeros(64, dtype=np.int16)
    current_dc = prev_dc + dc_diff
    zz[0] = current_dc
    
    idx = 1
    for run, val in ac_list:
        if run == 0 and val == 0: # EOB fleg
            break
        idx += run
        if idx < 64:
            zz[idx] = val
            idx += 1
    return zz, current_dc

def read_rle_file(filename):
    """
    @brief Reads RLE-encoded DCT blocks from a text file.

    @param filename Path to the RLE dump file
    @return List of zig-zag ordered DCT blocks
    """
    blocks = []
    prev_dc = 0
    
    if not os.path.exists(filename):
        return None

    with open(filename, 'r') as f:
        lines = f.readlines()
    
    i = 0
    while i < len(lines):
        line = lines[i].strip()
        if line.startswith('BLOCK'):
            i += 1
            if i >= len(lines): break
            
            dc_line = lines[i].strip()
            dc_diff = int(dc_line.replace('DC', '').strip())
            
            i += 1
            ac_pairs = []
            
            while i < len(lines):
                line = lines[i].strip()
                if line == 'EOB':
                    break
                parts = line.split()
                if len(parts) >= 2:
                    ac_pairs.append((int(parts[0]), int(parts[1])))
                i += 1
            
            zz, prev_dc = decode_rle_block(dc_diff, ac_pairs, prev_dc)
            blocks.append(zz)
            i += 1
        else:
            i += 1
            
    print(f"Učitano {len(blocks)} blokova iz {filename}")
    
    while len(blocks) < TOTAL_BLOCKS:
        blocks.append(np.zeros(64, dtype=np.int16))
        
    return blocks[:TOTAL_BLOCKS]


def reconstruct_image(blocks):
    """
    @brief Reconstructs a grayscale image from DCT blocks.

    The function applies inverse zig-zag scanning, dequantization,
    inverse DCT, and level shifting.

    @param blocks List of zig-zag ordered DCT coefficient blocks
    @return Reconstructed grayscale image as uint8 numpy array
    """
    img = np.zeros((IMG_HEIGHT, IMG_WIDTH), dtype=np.float32)
    
    for idx in range(TOTAL_BLOCKS):
        row = (idx // BLOCKS_X) * BLOCK_SIZE
        col = (idx % BLOCKS_X) * BLOCK_SIZE
        
        b = inverse_zigzag_8x8(blocks[idx])
        b = b * Q_TABLE
        b = idct2_block(b)
        b = b + 128
        
        img[row:row+8, col:col+8] = b
        
    return np.clip(img, 0, 255).astype(np.uint8)


def main():
    """
    @brief Main entry point of the reconstruction script.

    Loads RLE data, reconstructs the image, loads original reference images,
    computes reconstruction error, and visualizes results.
    """
    
    img_path = './images/test_stan.bmp'
    img_name = os.path.basename(img_path)  
    base_name, ext = os.path.splitext(img_name) 

    blocks = read_rle_file('C:/Users/23dej/Desktop/fakultet/Cetvrta godina/SDOS/ProjektniZadatak/Debug/rle_dump.txt')
    
    if blocks is None:
        print("Fajl 'rle_dump.txt' nije nađen.")
        return

    reconstructed = reconstruct_image(blocks)

    try:
        original_gray = Image.open(img_path).convert('L')
        original_gray = np.array(original_gray)
    except:
        original_gray = np.zeros((IMG_HEIGHT, IMG_WIDTH), dtype=np.uint8)

    try:
        original_rgb = Image.open(img_path).convert('RGB')
        original_rgb = np.array(original_rgb)
    except:
        original_rgb = np.zeros((IMG_HEIGHT, IMG_WIDTH, 3), dtype=np.uint8)
    
    reconstructed_filename = f"results/{base_name}_reconstructed.png"
    Image.fromarray(reconstructed).save(reconstructed_filename)
    
    diff = np.abs(original_gray.astype(int) - reconstructed.astype(int))

    plt.figure(figsize=(16, 8))  

    plt.subplot(2, 2, 1)
    plt.title("Original (Grayscale)")
    plt.imshow(original_gray, cmap='gray')
    plt.axis('off')

    plt.subplot(2, 2, 2)
    plt.title("Original (RGB)")
    plt.imshow(original_rgb)
    plt.axis('off')

    plt.subplot(2, 2, 3)
    plt.title(f"Dekompresovano {IMG_WIDTH}x{IMG_HEIGHT}")
    plt.imshow(reconstructed, cmap='gray')
    plt.axis('off')

    plt.subplot(2, 2, 4)
    plt.title(f"Razlika (MSE: {np.mean(diff**2):.2f})")
    plt.imshow(diff, cmap='hot')
    plt.axis('off')

    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    main()