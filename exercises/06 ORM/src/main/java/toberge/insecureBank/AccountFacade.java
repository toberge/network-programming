package toberge.insecureBank;

import java.util.List;

public interface AccountFacade {
    Account getOne(String accountNumber);
    void addOne(Account account);
    void updateOne(Account account);
    void deleteOne(String accountNumber);
    List<Account> getAll();
    List<Account> getAllLargerThan(double amount);
    List<Account> getAllByOwner(String owner);
    int getNumberOfAccounts();
}
