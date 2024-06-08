#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>

class MergeSort {

public:
    void Sort(std::vector<int>& nums) {
        MergeSortRecursive(nums, 0, nums.size() - 1);
    }

private:
    void MergeSortRecursive(std::vector<int>& nums, int left, int right) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            MergeSortRecursive(nums, left, mid);
            MergeSortRecursive(nums, mid + 1, right);
            Merge(nums, left, mid, right);
        }
    }

    void Merge(std::vector<int>& nums, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        std::vector<int> leftArray(n1), rightArray(n2);

        for (int i = 0; i < n1; ++i)
            leftArray[i] = nums[left + i];
        for (int j = 0; j < n2; ++j)
            rightArray[j] = nums[mid + 1 + j];

        int i = 0, j = 0, k = left;

        while (i < n1 && j < n2) {
            if (leftArray[i] <= rightArray[j]) {
                nums[k] = leftArray[i];
                ++i;
            }
            else {
                nums[k] = rightArray[j];
                ++j;
            }
            ++k;
        }

        while (i < n1) {
            nums[k] = leftArray[i];
            ++i;
            ++k;
        }

        while (j < n2) {
            nums[k] = rightArray[j];
            ++j;
            ++k;
        }
    }
};

int main() {
    setlocale(LC_ALL, "RUSSIAN");
    std::vector<int> numbers;
    std::cout << "Введите 10 целых чисел для сортировки:\n";

    for (int i = 0; i < 10; ++i) {
        int num;
        std::cin >> num;
        numbers.push_back(num);
    }

    MergeSort mergeSort;
    auto sortingFunc = [&mergeSort](std::vector<int>& nums) { mergeSort.Sort(nums); };

    std::thread mergeSortThread(sortingFunc, std::ref(numbers));
    mergeSortThread.join();

    std::cout << "Отсортированный массив:\n";
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}