#### Network
```java
public class Network {
  private PriorityQueue<Agent> queue;
  private Buffer<Packet> buffer;

  private static final String SINGLE_SENDER = "SSC";
  private static final String MULTI_SENDER = "MS";
  private static final String SINGLE_RECEIVER = "SR";
  private static final String MULTI_RECEIVER = "MR";

  public Network() {
    this.queue = new PriorityQueue<Agent>();
  }

  public void init(Scanner scanner) {
    this.buffer = new Buffer<Packet>(scanner.nextInt());
    int numAgent = scanner.nextInt();

    for (int i = 0; i < numAgent; i++) {
      String agentType = scanner.next();

      if (agentType.equals(Network.SINGLE_SENDER)) {
        int actionTime = scanner.nextInt();
        int increment = scanner.nextInt();
        int retries = scanner.nextInt();
        String message = scanner.next();
        queue.add(new SingleSender(actionTime, increment, retries, message));
      } else if (agentType.equals(Network.MULTI_SENDER)) {
        int actionTime = scanner.nextInt();
        int numMessages = scanner.nextInt();
        String[] messages = new String[numMessages];
        for (int j = 0; j < numMessages; j++) {
          String message = scanner.next();
          messages[j] = message;
        }
        queue.add(new MultiSender(actionTime, messages));
      } else if (agentType.equals(Network.SINGLE_RECEIVER)) {
        int actionTime = scanner.nextInt();
        int increment = scanner.nextInt();
        queue.add(new SingleReceiver(actionTime, increment));
      } else if (agentType.equals(Network.MULTI_RECEIVER)) {
        int actionTime = scanner.nextInt();
        int numMessages = scanner.nextInt();
        queue.add(new MultiReceiver(actionTime, numMessages));
      }
    }
  }

  public void run() {
    Agent agent = null;
    Agent next = null;
    while (!this.queue.isEmpty()) {
      agent = this.queue.poll();
      next = agent.act(this.buffer);

      System.out.println(agent);

      if (next != null) {
        this.queue.add(next);
      }
    }
  }
}
```

<div style="page-break-after: always;"></div>

#### Buffer

```java
public class Buffer<T extends Comparable<T>> {
  private T[] messages;
  private int endIndex;

  public Buffer(int size) {
    // rawtype and unchecked warnings can be suppressed because
    // T is guaranteed to be a subtype of Comparable
    @SuppressWarnings({"rawtypes", "unchecked"})
    T[] tmp = (T[]) new Comparable[size];
    this.messages = tmp;
    this.endIndex = 0;
  }

  public void send(T message) throws BufferFullException {
    if (this.endIndex >= this.messages.length) {
      throw new BufferFullException();
    }
    this.messages[this.endIndex] = message;
    this.endIndex++;
  }

  public T receive() throws BufferEmptyException { 
    if (this.endIndex == 0) {
      throw new BufferEmptyException();
    } else {
      T message = this.messages[0];
      int index = 0;
      for (int i = 1; i < this.endIndex; i++) {
        if (this.messages[i].compareTo(message) > 0) {
          message = this.messages[i];
          index = i;
        }
      }
      for (int i = index; i < this.endIndex - 1; i++) {
        this.messages[i] = this.messages[i + 1];
      }
      this.messages[this.endIndex - 1] = null;
      this.endIndex--;

      return message;
    }
  }
}
```

#### BufferEmptyException

```java
public class BufferEmptyException extends Exception {
  public BufferEmptyException() {
    super("Buffer is empty");
  }
}
```


#### Packet
```java
class Packet implements Comparable<Packet> {
  private String content;

  public Packet(String content) {
    this.content = content;
  }
  
  @Override
  public int compareTo(Packet p) {
    return p.content.length() - this.content.length();
  }

  @Override
  public String toString() {
    return this.content;
  }
}
```