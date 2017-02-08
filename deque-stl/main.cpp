#include <iostream>
#include <deque>
#include <algorithm>

using namespace std;

void printKMax(int arr[], int n, int k) {
   int i = 0, max, dropped;
   deque<int> dq(arr + i, arr + i + k);

   max = *max_element(dq.begin(), dq.end());

   while((k + i) <= n) {
      cout << max << " ";
      dropped = dq.front();
      dq.pop_front();

      dq.push_back(arr[i++ + k]);

      if(dq.back() > max)
         max = dq.back();

      if(max == dropped)
      {
         max = *max_element(dq.begin(), dq.end());
      }
   }

   cout << endl;
}

int main(){

   int t;
   cin >> t;
   while(t>0) {
      int n,k;
      cin >> n >> k;
      int i;
      int arr[n];
      for(i=0;i<n;i++)
         cin >> arr[i];
      printKMax(arr, n, k);
      t--;
   }
   return 0;
}

