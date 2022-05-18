// Ejercicio 2

/*
PRAM

start_time = now()
for i in range(n-1) pardo:
    message = get_message(i)
    if i!= n-1:
        send_message(i,i+1,message)
    else:
        send_message(i,0,message)
end_time = now()
duration = end_time - start_time
time_per_process = duration / n

*/

#include <iostream>
#include <mpi.h>
#include <vector>
#include <numeric>
#include <fstream>

using namespace std;

/*
template <typename T> send_message(T i, T j, T message) {
    cout << "Enviando mensaje de " << i << " a " << j << ": " << message << endl;
}
*/

int main(int argc, char *argv[]) {
    
    int rank, n;
    char message = '4';
    /*
    char 1 byte
    int 4 bytes
    double 8 bytes
    */

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n);
    double final_time = 0;

    double start_time = 0.0;
    double end_time = 0.0;
    if (rank == 0){
        start_time = MPI_Wtime();
        MPI_Send(&message, 1, MPI_CHAR, 1, rank, MPI_COMM_WORLD);
        end_time = MPI_Wtime();
        final_time = (end_time - start_time) * 1e6;
        cout << "Message sent from 0 to 1" << endl;
    } else if (rank != n-1){
        MPI_Recv(&message, 1, MPI_CHAR, rank-1, rank-1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        start_time = MPI_Wtime();
        MPI_Send(&message, 1, MPI_CHAR, rank+1, rank, MPI_COMM_WORLD);
        end_time = MPI_Wtime();
        final_time = (end_time - start_time) * 1e6;
        cout << "Message sent from " << rank << " to " << rank+1 << endl;
    } else{
        MPI_Recv(&message, 1, MPI_CHAR, rank-1, rank-1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        start_time = MPI_Wtime();
        MPI_Send(&message, 1, MPI_CHAR, 0, rank, MPI_COMM_WORLD);
        end_time = MPI_Wtime();
        final_time = (end_time - start_time) * 1e6;
        cout << "Message sent from " << rank << " to 0" << endl;
    }
    double final_time_sum = 0.0;
    MPI_Reduce(&final_time, &final_time_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        fstream f;
        f.open("test_char_64.txt", ios::app);
        //if(f==NULL){printf("failed to open file: permission issue ?\n");exit(1);}
        //fprintf(f,"%.2f \n",final_time_sum / double(n));
        f << final_time_sum / double(n);
        f << '\n';
    f.close();
    }

    MPI_Finalize();
    
    cout << "Total time per process: " << final_time_sum / double(n) << " from rank: " << rank << endl;

    return 0;
}
