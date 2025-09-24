use std::error::Error;
// use std::io::{BufRead, BufReader, Write};
// use std::net::{TcpListener, TcpStream};
use tokio::io::{AsyncBufReadExt, AsyncWriteExt, BufReader};
use tokio::net::{TcpListener, TcpStream};
use std::sync::mpsc;

use crate::task::{Task, TaskType};

pub trait ServerTrait {
    fn start_server(
        &self,
        address: String,
        tx: mpsc::Sender<Result<(), Box<dyn Error + Send>>>,
    );
}

pub struct Server;

impl ServerTrait for Server {
    fn start_server(
        &self,
        address: String,
        tx: mpsc::Sender<Result<(), Box<dyn Error + Send>>>,
    ) {
        println!("Starting the server");
        let threaded_rt = tokio::runtime::Builder::new_multi_thread()
            .max_blocking_threads(12)
            .worker_threads(4)
            .enable_all()
            .build()
            .unwrap();

        threaded_rt.block_on(async {
            let listener = match TcpListener::bind(address).await {
                Ok(listener) => {
                    tx.send(Ok(())).unwrap();
                    listener
                },
                Err(e) => {
                    println!("here {}", e);
                    tx.send(Err(Box::new(e))).unwrap();
                    return;
                }
            };
            
            loop {
                let result = listener.accept().await;
                match result {
                    Ok((stream, _addr)) => {
                        // Spawns task for each client connection
                        tokio::spawn(async move{
                            Self::handle_connection(stream).await;
                        });
                    }
                    Err(e) => {
                        eprintln!("Error accepting connection: {}", e);
                    }
                }   
            }
        });
    }
}

impl Server {
    async fn handle_connection(mut stream: TcpStream) {
        // Continually reads and handles client input
        loop {
            let mut buf_reader = BufReader::new(&mut stream);
            let mut line = String::new();
            match buf_reader.read_line(&mut line).await {
                Ok(0) => {
                    return;
                }
                Ok(_) => {
                    let response = Self::get_task_value(line).await;
                    if let Some(r) = response {
                        stream.write(&[r]).await.unwrap();
                    }
                }
                Err(e) => {
                    eprintln!("Unable to get command due to: {}", e);
                    return;
                }
            }
        }
    }

    async fn concurrent_cpu(task_type: u8, seed: u64) -> Option<u8> {
        tokio::task::spawn_blocking(move || Task::execute(task_type, seed)).await.ok()
    }

    async fn get_task_value(buf: String) -> Option<u8> {
        let numbers: Vec<&str> = buf.trim().split(':').collect();
        let task_type = match numbers.first().unwrap().parse::<u8>() {
            Ok(val) => val,
            Err(_) => return None,
        };
        
        let seed = match numbers.last().unwrap().parse::<u64>() {
            Ok(val) => val,
            Err(_) => return None,
        };

        match TaskType::from_u8(task_type).unwrap() {
            TaskType::CpuIntensiveTask => {
                Self::concurrent_cpu(task_type, seed).await},
            TaskType::IOIntensiveTask => Some(Task::execute_async(task_type, seed).await),            
        }
    }
}
