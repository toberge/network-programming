package toberge.insecureBank;

import java.util.List;

public class AccountFacadeImpl implements AccountFacade {

    private final AccountDAO dao;

    public AccountFacadeImpl(AccountDAO dao) {
        this.dao = dao;
    }

    public Account getOne(String accountNumber) {
        return dao.getOne(accountNumber);
    }

    public synchronized void addOne(Account account) {
        dao.addOne(account);
    }

    public synchronized void updateOne(Account account) {
        dao.updateOne(account);
    }

    public synchronized void deleteOne(String accountNumber) {
        dao.deleteOne(accountNumber);
    }

    public List<Account> getAll() {
        return dao.getAll();
    }

    public List<Account> getAllByOwner(String owner) {
        return dao.getAllByOwner(owner);
    }

    public List<Account> getAllLargerThan(double amount) {
        return dao.getAllLargerThan(amount);
    }

    public int getNumberOfAccounts() {
        return dao.getNumberOfAccounts();
    }
}
