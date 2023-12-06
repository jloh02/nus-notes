## Lazy.java
```java
package cs2030s.fp;

import javax.swing.TransferHandler;

public class Lazy<T> {
  private T val;
  private boolean flag;
  private Producer<? extends T> prod;

  private Lazy(T val) {
    this.val = val;
    this.flag = true;
    this.prod = () -> val;
  }

  private Lazy(Producer<? extends T> prod) {
    this.val = null;
    this.flag = false;
    this.prod = prod;
  }

  public T get() {
    if (!this.flag) {
      this.val = this.prod.produce();
      this.flag = true;
    }
    return this.val;
  }

  public static <T> Lazy<T> of(T val) {
    return new Lazy<T>(val);
  }

  public static <T> Lazy<T> of(Producer<? extends T> prod) {
    return new Lazy<T>(prod);
  }

  public <U> Lazy<U> map(Transformer<? super T, ? extends U> fn) {
    return Lazy.of(() -> fn.transform(this.get()));
  }

  public <U> Lazy<U> flatMap(Transformer<? super T, ? extends Lazy<? extends U>> fn) {
    return Lazy.of(() -> fn.transform(this.get()).get());
  }

  public Lazy<Boolean> filter(BooleanCondition<? super T> pred) {
    return Lazy.of(() -> pred.test(this.get()));
  }

  @Override
  public String toString() {
    if (this.flag) {
      return String.valueOf(this.val);
    }
    return "?";
  }

  @Override
  public boolean equals(Object obj) {
    if (obj instanceof Lazy<?>) {
      Lazy<?> lazy = (Lazy<?>) obj;
      return this.get() == lazy.get() || this.get().equals(lazy.get());
    }
    return false;
  }
}
```

## Maybe.java
```java
package cs2030s.fp;

public class Maybe<T> {
  private final T content;
  private static final Maybe<?> NONE = new None<>();

  private Maybe(T content) {
    this.content = content;
  }

  public static <T> Maybe<T> some(T content) {
    return new Maybe<T>(content);
  }

  public static <T> Maybe<T> none() {
    @SuppressWarnings("unchecked")
    Maybe<T> res = (Maybe<T>) NONE;
    return res;
  }

  public static <T> Maybe<T> of(T content) {
    if (content == null) {
      return Maybe.<T>none();
    }
    return new Maybe<T>(content);
  }

  @Override
  public String toString() {
    return "[" + this.content + "]";
  }

  @Override
  public boolean equals(Object obj) {
    if (obj == this) {
      return true;
    }
    if (obj instanceof None<?>) {
      return false;
    }
    if (obj instanceof Maybe<?>) {
      Maybe<?> maybe = (Maybe<?>) obj;
      if (this.content == null) {
        return maybe.content == null;
      }
      return this.content.equals(maybe.content);
    }
    return false;
  }

  public <U> Maybe<U> map(Transformer<? super T, ? extends U> fn) {
    return Maybe.some(fn.transform(this.content));
  }

  public Maybe<T> filter(BooleanCondition<? super T> pred) {
    if (this.content == null || pred.test(this.content) == false) {
      return Maybe.none();
    } else {
      return this;
    }
  }

  public <U> Maybe<U> flatMap(Transformer<? super T, ? extends Maybe<? extends U>> fn) {
    @SuppressWarnings("unchecked")
    Maybe<U> res = (Maybe<U>) fn.transform(this.content);
    return res;
  }

  public T orElse(Producer<? extends T> prod) {
    return this.content;
  }

  public void ifPresent(Consumer<? super T> cons) {
    cons.consume(this.content);
  }

  private static class None<T> extends Maybe<T> {
    private None() {
      super(null);
    }

    @Override
    public String toString() {
      return "[]";
    }

    @Override
    public boolean equals(Object obj) {
      if (obj == this) {
        return true;
      }
      if (obj instanceof None<?>) {
        return true;
      }
      return false;
    }

    @Override
    public <U> Maybe<U> map(Transformer<? super T, ? extends U> fn) {
      return Maybe.<U>none();
    }

    @Override
    public Maybe<T> filter(BooleanCondition<? super T> pred) {
      return Maybe.none();
    }

    @Override
    public <U> Maybe<U> flatMap(Transformer<? super T, ? extends Maybe<? extends U>> fn) {
      return Maybe.none();
    }

    @Override
    public T orElse(Producer<? extends T> prod) {
      return prod.produce();
    }

    @Override
    public void ifPresent(Consumer<? super T> cons) {
      // do nothing
    }
  }
}
```

## InfiniteList.java
```java
package cs2030s.fp;

import java.util.ArrayList;
import java.util.List;

/**
 * The InfiniteList class that may contain an arbitrary number of
 * elements.  The class is a generic class.
 * 
 * @author Jonathan Loh (Group 14B)
 * @param <T> The type to be stored in the InfiniteList.
 */
public class InfiniteList<T> {
  /**
   * Lazy stores the first value of InfiniteList.
   */
  private final Lazy<Maybe<T>> head;

  /**
   * Recursive object to store remaining values in InfiniteList.
   */
  private final Lazy<InfiniteList<T>> tail;
  /**
   * Empty list implementation.
   */
  private static final InfiniteList<?> SENTINEL = (InfiniteList<?>) new Sentinel();

  /**
   * Private constructor to set all fields null.
   */
  private InfiniteList() {
    this.head = null;
    this.tail = null;
  }

  /**
   * Private constructor given head and tail.
   * 
   * @param head The Lazy instance containing the head of the InfiniteList.
   * @param tail The Lazy instance to produce the tail of the InfiniteList.
   */
  private InfiniteList(Lazy<Maybe<T>> head, Lazy<InfiniteList<T>> tail) {
    this.head = head;
    this.tail = tail;
  }

  /**
   * Generates an InfiniteList.  Given a producer that produces
   * a value x, generate the list as [x, x, x, ...]
   * 
   * @param <T> The type to be stored in the InfiniteList.
   * @param prod The producer to produce the value in the InfiniteList.
   * @return The created InfiniteList.
   */
  public static <T> InfiniteList<T> generate(Producer<T> prod) {
    return new InfiniteList<>(
        Lazy.of(() -> Maybe.some(prod.produce())),
        Lazy.of(() -> InfiniteList.generate(prod))
        );
  }

  /**
   * Generate an InfiniteList.  Given x and a lambda f, 
   * generate the list as [x, f(x), f(f(x)), f(f(f(x))), ...]
   * 
   * @param <T> The type to be stored in the InfiniteList.
   * @param init The first element.
   * @param next The transformation function on the element.
   * @return The created InfiniteList.
   */
  public static <T> InfiniteList<T> iterate(T init,
      Transformer<? super T, ? extends T> next) {
    return new InfiniteList<>(
        Lazy.of(Maybe.some(init)),
        Lazy.of(() -> InfiniteList.iterate(next.transform(init), next))
        );
  }

  /**
   * Generate an InfiniteList.  This is an empty InfiniteList.
   * 
   * @param <T> The type to be stored in the InfiniteList.
   * @return The created InfiniteList.
   */
  public static <T> InfiniteList<T> sentinel() {
    // Can be safely typecasted to InfiniteList as SENTINEL is independent
    // of generic type T and Sentinel is a subtype of InfiniteList
    @SuppressWarnings("unchecked")
    InfiniteList<T> res = (InfiniteList<T>) SENTINEL;
    return res;
  }

  /**
   * Get head (first item) of InfiniteList.
   *
   * @return InfiniteList head
   */
  public T head() {
    return this.head.get().orElse(() -> this.tail.get().head());
  }


  /**
   * Get remaining items of InfiniteList.
   *
   * @return recursive InfiniteList tail
   */
  public InfiniteList<T> tail() {
    return this.head.get()
      .map(x -> this.tail.get())
      .orElse(() -> this.tail.get().tail());
  }

  /**
   * Map each item in InfiniteList using a Transformer.
   *
   * @param <U> The type to map to
   * @param fn Transformer function to map the values in InfiniteList
   * @return InfiniteList with each item mapped using fn
   */
  public <U> InfiniteList<U> map(Transformer<? super T, ? extends U> fn) {
    return new InfiniteList<>(
        this.head.map(mHead -> mHead.map(fn)),
        this.tail.map(mTail -> mTail.map(fn))
        );
  }

  /**
   * Filter each item in InfiniteList using a BooleanCondition.
   *
   * @param fn BooleanCondition to determine if a value should be included in the list
   * @return InfiniteList with filtered values where BooleanCondition is true
   */
  public InfiniteList<T> filter(BooleanCondition<? super T> fn) {
    return new InfiniteList<>(
        this.head.map(mHead -> mHead.filter(fn)),
        this.tail.map(mTail -> mTail.filter(fn))
        );
  }

  /**
   * Limits the InfiniteList to a finite length.
   *
   * @param n length to limit
   * @return InfiniteList with limited finite length
   */
  public InfiniteList<T> limit(long n) {
    if (n <= 0) {
      return InfiniteList.sentinel();
    }
    return new InfiniteList<>(
        this.head,
        this.tail.map(mTail -> mTail.limit(
            Maybe.none().equals(this.head.get()) ? n : n - 1
            )
          )
        );
  }

  /**
   * Converts the InfiniteList to a Collections API ArrayList.
   *
   * @return List representation of InfiniteList
   */
  public List<T> toList() {
    ArrayList<T> list = new ArrayList<>();
    this.head.get().ifPresent(x -> list.add(x));
    list.addAll(this.tail.get().toList());
    return list;
  }

  /**
   * Truncates InfiniteList to the largest subarray containing the first element such that 
   * all items in the subarray satisfy the given BooleanCondition.
   *
   * @param pred BooleanCondition to determine where the cutoff point should be
   * @return truncated InfiniteList 
   */
  public InfiniteList<T> takeWhile(BooleanCondition<? super T> pred) {
    T headVal = this.head.get().orElse(() -> null);

    if (headVal == null) {
      return this.tail.map(mTail -> mTail.takeWhile(pred)).get();
    }

    Lazy<Boolean> test = Lazy.of(() -> pred.test(headVal)); 

    return new InfiniteList<>(
        Lazy.of(() -> test.get() ? this.head.get() : Maybe.none()), 
        Lazy.of(() -> test.get() ? 
            this.tail.map(mTail -> mTail.takeWhile(pred)).get() : 
            InfiniteList.<T>sentinel())
        );
  }


  /**
   * Reduces InfiniteList from last index using the given accumulator.
   *
   * @param <U> The type to accumulate the output
   * @param id Initial value to pass to the accumulator
   * @param acc Combiner to perform accumulation
   * @return Reduced value using the acc accumulator from last index
   */
  public <U> U foldRight(U id, Combiner<? super T, U, U> acc) {
    return acc.combine(this.head(), this.tail().foldRight(id, acc));
  }

  /**
   * Get string representation of list.
   *
   * @return InfiniteList as a String
   */
  @Override
  public String toString() {
    return "[" + this.head + " " + this.tail + "]";
  }

  /**
   * Determines if InfiniteList is a Sentinel.
   *
   * @return false
   */
  public boolean isSentinel() {
    return false;
  }

  /**
   * The internal nested Sentinel class is used to determine empty lists.
   * It extends InfiniteList
   */
  private static class Sentinel extends InfiniteList<Object> {
    @Override
    public Object head() {
      throw new java.util.NoSuchElementException();
    }

    @Override
    public InfiniteList<Object> tail() {
      throw new java.util.NoSuchElementException();
    }

    @Override
    public <R> InfiniteList<R> map(Transformer<Object, ? extends R> mapper) {
      return InfiniteList.<R>sentinel();
    }

    @Override
    public InfiniteList<Object> filter(BooleanCondition<Object> predicate) {
      return InfiniteList.<Object>sentinel();
    }

    @Override
    public InfiniteList<Object> limit(long n) {
      return InfiniteList.sentinel();
    }

    @Override
    public List<Object> toList() {
      return new ArrayList<>();
    }

    @Override
    public InfiniteList<Object> takeWhile(BooleanCondition<Object> pred) {
      return InfiniteList.sentinel();
    }

    @Override
    public <U> U foldRight(U id, Combiner<Object, U, U> acc) {
      return id;
    }

    @Override
    public String toString() {
      return "~";
    }

    @Override
    public boolean isSentinel() {
      return true;
    }
  }
}
```

## Streams
### Lab to Stream Abstractions
|CS2030S|java.util.function|
|---|---|
|`BooleanCondition<T>::test`|[`Predicate<T>::test`](https://docs.oracle.com/en/java/javase/17/docs/api/java.base/java/util/function/Predicate.html#test(T))|
|`Producer<T>::produce`|[`Supplier<T>::get`](https://docs.oracle.com/en/java/javase/17/docs/api/java.base/java/util/function/Supplier.html#get())|
|`Consumer<T>::consume`|[`Consumer<T>::accept`](https://docs.oracle.com/en/java/javase/17/docs/api/java.base/java/util/function/Consumer.html#accept(T))|
|`Transformer<T,R>::transform`|[`Function<T,R>::apply`](https://docs.oracle.com/en/java/javase/17/docs/api/java.base/java/util/function/Function.html#apply(T))|
|`Transformer<T,T>::transform`|[`UnaryOperator<T>::apply`](https://docs.oracle.com/en/java/javase/17/docs/api/java.base/java/util/function/UnaryOperator.html)|

|CS2030S|Java version|
|---|---|
|`Some<T>`  <br>(_before_ `Maybe<T>`)|N/A|
|`Maybe<T>`|[`java.util.Optional<T>`](https://docs.oracle.com/en/java/javase/17/docs/api/java.base/java/util/Optional.html)|
|`Lazy<T>`|N/A|
|`InfiniteList<T>`|[`java.util.stream.Stream<T>`](https://docs.oracle.com/en/java/javase/17/docs/api/java.base/java/util/stream/Stream.html)|

```java
// Building a Stream
Stream.of(1, 2, 3); // Using the static factory method
Stream.generate(() -> 1); // Using generate method for infinite streams
Arrays.stream(array); // Converting an array into a Stream
list.stream(); // Converting a List into a Stream
Files.lines(path); // Example of other APIs returning a Stream

// Terminal Operations
stream.forEach(System.out::println); // forEach as a terminal operation

// Intermediate Stream Operations
stream.map(function); // Map operation
stream.filter(predicate); // Filter operation
stream.flatMap(function); // FlatMap operation

// FlatMapping a Stream
stream.map(x -> x.lines()); // Returns a stream of streams
stream.flatMap(x -> x.lines()); // Returns a stream of strings

// Stateful and Bounded Operations
stream.sorted(); // Sorts elements in the stream
stream.sorted(comparator); // Sorts elements with a custom comparator
stream.distinct(); // Returns a stream with distinct elements

// Truncating an Infinite List
stream.limit(n); // Takes the first n elements
stream.takeWhile(predicate); // Takes elements until the predicate becomes false

// Peeking with a Consumer
stream.peek(System.out::println); // Applies a lambda on a "fork" of the stream

// Reducing a Stream
stream.reduce(identity, accumulator); // Reduce operation to obtain a single value

// Element Matching
stream.noneMatch(predicate); // Returns true if none of the elements pass the predicate
stream.allMatch(predicate); // Returns true if every element passes the predicate
stream.anyMatch(predicate); // Returns true if at least one element passes the predicate

// Consumed Once
// Note: Streams can only be operated on once; recreate the stream if needed

// Example: Is this a prime?
IntStream.range(2, x).noneMatch(i -> x % i == 0); // Using IntStream to check for prime

// Example: First 500 primes
IntStream.iterate(2, x -> x + 1)
    .filter(x -> isPrime(x))
    .limit(500)
    .forEach(System.out::println); // Using streams for prime number generation

// Caution: Avoid Overusing Streams
// Not all loops can be elegantly translated into streams; consider readability and complexity
```

## Loggable
```java
class Loggable<T> {
  private final T value;
  private final String log;

  private Loggable(T value, String log) {
    this.value = value;
    this.log = log;
  }

  public static <T> Loggable<T> of(T value) {
    return new Loggable<>(value, "");
  }

  public <R> Loggable<R> flatMap(Transformer<? super T, ? extends Loggable<? extends R>> transformer) {
    Loggable<? extends R> l = transformer.transform(this.value);
    return new Loggable<>(l.value, l.log + this.log);
  }

  public String toString() {
    return "value: " + this.value + ", log: " + this.log;
  }
}
```

<div style="page-break-after: always;"></div>

## Bank.java (pe2pracA)
```java
import cs2030s.fp.Maybe;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.stream.Stream;

/**
 * Represents a bank with multiple accounts and customers.
 */
public class Bank {

  private Map<Integer, Account> accounts;

  /**
   * Constructs a new Bank with the given list of accounts and customers.
   *
   * @param accounts  a map of account numbers and their associated accounts
   * @param customers an list of customers
   */
  public Bank(Map<Integer, Account> accounts) {
    this.accounts = accounts;
  }

  /**
   * Returns a stream of all accounts in the bank.
   *
   * @return a stream containing all accounts in the bank
   */
  private Stream<Account> getAccountStream() {
    return this.accounts.values().stream();
  }

  /**
   * Returns the account with the specified account number.
   *
   * @param number the account number to search for
   * @return the Account with the specified account number, or null if not found
   */
  private Maybe<Account> findAccount(int number) {
    // Map::get returns null if the account does not exist
    return Maybe.of(this.accounts.get(number));
  }

  /**
   * Calculates the total money in the bank across all accounts.
   *
   * @return the total money in the bank
   */
  public double totalMoneyInBank() {
    return this.accounts.values().stream()
      .filter(acc -> !acc.isClosed())
      .map(acc -> acc.getBalance())
      .reduce(0.0, (x, y) -> x + y);
  }

  /**
   * Transfers the specified amount of money between accounts.
   *
   * @param from   the account number to transfer money from
   * @param to     the account number to transfer money to
   * @param amount the amount of money to transfer
   */
  public void transfer(int from, int to, double amount) {
    Maybe<Account> fromAcc = findAccount(from).filter(x -> x.getBalance() > 0 && !x.isClosed());
    Maybe<Account> toAcc = findAccount(to).filter(x -> !x.isClosed());

    fromAcc.flatMap(f -> toAcc.map(t -> f.transferTo(t, amount)))
      .ifPresent(pair -> accounts.put(from, pair.getFirst()));
    fromAcc.flatMap(f -> toAcc.map(t -> f.transferTo(t, amount)))
      .ifPresent(pair -> accounts.put(to, pair.getSecond()));
  }

  /**
   * Deposits the specified amount of money into the specified account.
   *
   * @param number the account number to deposit money into
   * @param amount the amount of money to deposit
   */
  public void deposit(int number, double amount) {
    findAccount(number)
      .filter(a -> !a.isClosed())
      .ifPresent(acc -> this.accounts.put(number, acc.deposit(amount)));
  }

  /**
   * Withdraws the specified amount of money from the specified account.
   *
   * @param number the account number to withdraw money from
   * @param amount the amount of money to withdraw
   */
  public void withdraw(int number, double amount) {
    findAccount(number)
      .filter(a -> !a.isClosed())
      .ifPresent(acc -> this.accounts.put(number, acc.withdraw(amount)));
  }

  /**
   * Combines two accounts and updates the accounts list.
   *
   * @param a the account number of the first account to combine
   * @param b the account number of the second account to combine
   */
  public void combineBankAccount(int a, int b) {
    findAccount(a)
      .filter(x -> !x.isClosed())
      .flatMap(c -> findAccount(b).filter(x -> x.isClosed()).map(d -> c.combine(d)))
      .map(x -> accounts.put(a, x))
      .map(x -> accounts.remove(b));
  }

  /**
   * Return the details of all accounts in the bank as a string.
   */
  public String allAccounts() {
    ArrayList<Account> sortedAccounts = new ArrayList<>(this.accounts.values());
    // Double.compare(d1,d2) returns 0 if d1 is numerically equal to d2;
    // a value less than 0 if d1 is numerically less than d2;
    // and a value greater than 0 if d1 is numerically greater than d2
    sortedAccounts.sort((x, y) -> Double.compare(y.getBalance(), x.getBalance()));
    return sortedAccounts.stream()
      .filter(acc -> !acc.isClosed())
      .map(Account::toString)
      .reduce("", (x, y) -> x + y + "\n");
  }

  /**
   * Closes the account with the specified account number and removes it from the accounts list.
   *
   * @param number the account number of the account to close
   */
  public void closeAccount(int number) {
    findAccount(number).ifPresent(acc -> this.accounts.put(number, acc.close()));
  }

  /**
   * Checks if this bank is equals to another bank, by comparing the accounts.
   *
   * @param o the object to compare against
   * @return true if the two banks are equal; false otherwise.
   */
  @Override
  public boolean equals(Object o) {
    if (o instanceof Bank) {
      Bank c = (Bank) o;
      return c.accounts.equals(this.accounts);
    }
    return false;
  }

  /**
   * Return a string representation of the map of accounts.
   *
   * @return a string representation of this bank.
   */
  @Override
  public String toString() {
    return this.accounts.toString();
  }
}
```
## Monads

- **Monad Pattern:**
    - Monad classes have:
        - `of` method for initialization of value and side information.
        - `map` method to update the value.
        - `flatMap` method to update both value and side information.
        - Different side information for each class (`Maybe<T>`, `Lazy<T>`, `Loggable<T>`).
- **Monad Generalization:**
    - Abstract `Loggable<T>` into `Monad<T, S>` with a generic type `S` for context (side-information).
```java
class Monad<T, S> {
  private final T content; // value to be operated on
  private final S context; // side-information

  // ... other methods ...

  public <U> Monad<U, S> map(Transformer<? super T, ? extends U> transformer) {
    // preserve context
    U content = transformer.transform(this.content);
    return new Monad(content, this.context);
  }

  public <U> Monad<U, S> flatMap(Transformer<? super T, ? extends Monad<? extends U, ? extends S>> transformer) {
    // compose context using static method
    Monad<? extends U, ? extends S> next = transformer.transform(this.content);
    return Monad.compose(this, next);
  }

  private static <T, U, S> Monad<U, S> compose(Monad<T, S> prev, Monad<? extends U, ? extends S> next) {
    // ... code omitted ...
  }
}
```

#### Monad Laws
- **Monad Laws:**
    - Monads must follow three laws:
         1. **Left Identity Law:**
        - `Monad.of(x).flatMap(x -> f(x))` must be the same as `f(x)`
        2. **Right Identity Law:**        
        - `monad.flatMap(x -> Monad.of(x))` must be the same as `monad`
        3. **Associative Law:**        
        - `monad.flatMap(x -> f(x)).flatMap(x -> g(x))` must be the same as `monad.flatMap(x -> f(x).flatMap(y -> g(y)))`

#### `flatMap` using `map`
- **Functional Relationship:**
    - `flatMap` should invoke `map` for better definition.
    - Ensures that monads are functors with additional properties.

```java
public <U> Monad<U, S> flatMap(Transformer<? super T, ? extends Monad<? extends U, ? extends S>> fn) {
  // invoke map and combine using static method
  Monad<Monad<? extends U, ? extends S>, S> monad = this.map(fn);
  return Monad.combine(monad);
}

private static <T, U, S> Monad<U, S> combine(Monad<Monad<? extends U, ? extends S>, S> monad) {
  // ... code omitted ...
}
```
### Functors
#### Two Laws of Functors
- **Preserving Identity:**
    - `functor.map(x -> x)` is the same as `functor`
- **Preserving Composition:**    
    - `functor.map(x -> f(x)).map(x -> g(x))` is the same as `functor.map(x -> g(f(x)))`
- **Inference:**    
    - Map method cannot modify the context or change it into the identity context
#### Classes as Functors
- Functors in `cs2030s.fp`: `Lazy<T>`, `Maybe<T>`, and `InfiniteList<T>` are functors.