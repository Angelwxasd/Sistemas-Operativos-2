import java.util.Scanner;

public class MemorySimulator {

    private static byte[] memoryMap; // Array to store the memory map as bits
    private static int totalBlocks; // Total number of blocks in memory
    private static int blockSize; // Size of each block in bytes

    public static void initializeMemory(int memorySize, int blockSize) {
        MemorySimulator.blockSize = blockSize;
        totalBlocks = memorySize / blockSize;
        memoryMap = new byte[(int) Math.ceil(totalBlocks / 8.0)];
    }

    public static boolean allocateMemory(int processId, int blocksNeeded) {
        int consecutiveBlocks = 0;
        int startIndex = -1;

        // Search for consecutive free blocks
        for (int i = 0; i < totalBlocks; i++) {
            if (isBlockFree(i)) {
                if (consecutiveBlocks == 0) {
                    startIndex = i;
                }
                consecutiveBlocks++;
                if (consecutiveBlocks == blocksNeeded) {
                    break;
                }
            } else {
                consecutiveBlocks = 0;
            }
        }

        if (consecutiveBlocks < blocksNeeded) {
            System.out.println("Error: Not enough consecutive blocks available.");
            return false;
        }

        // Mark blocks as used
        for (int i = startIndex; i < startIndex + blocksNeeded; i++) {
            markBlockUsed(i);
        }

        System.out.println("Process " + processId + " allocated from block " + startIndex + " to block " + (startIndex + blocksNeeded - 1));
        return true;
    }

    public static void freeMemory(int startBlock, int blocksToFree) {
        for (int i = startBlock; i < startBlock + blocksToFree; i++) {
            markBlockFree(i);
        }
        System.out.println("Memory freed from block " + startBlock + " to block " + (startBlock + blocksToFree - 1));
    }

    private static boolean isBlockFree(int blockIndex) {
        int byteIndex = blockIndex / 8;
        int bitIndex = blockIndex % 8;
        return (memoryMap[byteIndex] & (1 << bitIndex)) == 0;
    }

    private static void markBlockUsed(int blockIndex) {
        int byteIndex = blockIndex / 8;
        int bitIndex = blockIndex % 8;
        memoryMap[byteIndex] |= (1 << bitIndex);
    }

    private static void markBlockFree(int blockIndex) {
        int byteIndex = blockIndex / 8;
        int bitIndex = blockIndex % 8;
        memoryMap[byteIndex] &= ~(1 << bitIndex);
    }

    public static void printMemoryMap() {
        System.out.print("Memory Map: ");
        for (int i = 0; i < totalBlocks; i++) {
            System.out.print(isBlockFree(i) ? "0" : "1");
            if ((i + 1) % 8 == 0) {
                System.out.print(" ");
            }
        }
        System.out.println();
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Enter memory size in KB: ");
        int memorySize = scanner.nextInt() * 1024; // Convert KB to bytes
        System.out.print("Enter block size in KB: ");
        int blockSize = scanner.nextInt() * 1024; // Convert KB to bytes

        initializeMemory(memorySize, blockSize);

        while (true) {
            System.out.println("\nOptions:");
            System.out.println("1. Allocate memory for a process");
            System.out.println("2. Free memory");
            System.out.println("3. Print memory map");
            System.out.println("4. Exit");
            System.out.print("Choose an option: ");
            int choice = scanner.nextInt();

            switch (choice) {
                case 1:
                    System.out.print("Enter process ID: ");
                    int processId = scanner.nextInt();
                    System.out.print("Enter number of blocks needed: ");
                    int blocksNeeded = scanner.nextInt();
                    allocateMemory(processId, blocksNeeded);
                    break;
                case 2:
                    System.out.print("Enter starting block to free: ");
                    int startBlock = scanner.nextInt();
                    System.out.print("Enter number of blocks to free: ");
                    int blocksToFree = scanner.nextInt();
                    freeMemory(startBlock, blocksToFree);
                    break;
                case 3:
                    printMemoryMap();
                    break;
                case 4:
                    System.out.println("Exiting...");
                    scanner.close();
                    return;
                default:
                    System.out.println("Invalid option. Try again.");
            }
        }
    }
}
