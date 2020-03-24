# Exercise 6 - Object-Relational Mapping

## Part 1

Optimistic locking requires a "version" field in each database table.  
Each modification increments the version number by one –
meaning that if data is read, modified and ready to be stored,
seeing a higher version number will raise an exception to prevent invalid state.  
Rather than being an explicit lock, this is a way to detect concurrent modification
and prevent the system from ending up in an incorrect state.

### Example:

Thread 1 and 2 both read accounts A and B. Both find {version = 0}.  
*Time passes.*  
Thread 1 writes the transaction of 1000$ A -> B and increments {version = 1}.  
*Time passes.*  
Thread 2 attempts to transfer 1000$ A -> B but sees {version != 0}
and cancels the transaction, retrieves data again and *then* transfers 1000$ A -> B {version = 2}

## Part 2

See
[ImplementationTest.java](src/main/java/toberge/insecureBank/ImplementationTest.java)
for the requested test

## Part 3

See
[RaceCondition.java](src/main/java/toberge/insecureBank/RaceCondition.java)
for a demonstration of a race condition

## Part 4

See
[OptimisticLockingTest.java](src/main/java/toberge/lockedBank/OptimisticLockingTest.java)
for actual use of optimistic locking
