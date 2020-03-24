package toberge.lockedBank;

import java.util.List;

public class LockedAccountFacadeImpl {
    private final LockedAccountDAO dao;

    public LockedAccountFacadeImpl(LockedAccountDAO dao) {
        this.dao = dao;
    }

    public LockedAccount getOne(String accountNumber) {
        return dao.getOne(accountNumber);
    }

    public synchronized void addOne(LockedAccount account) {
        dao.addOne(account);
    }

    public synchronized void updateOne(LockedAccount account) {
        dao.updateOne(account);
    }

    public synchronized void deleteOne(String accountNumber) {
        dao.deleteOne(accountNumber);
    }

    public List<LockedAccount> getAll() {
        return dao.getAll();
    }

    public List<LockedAccount> getAllByOwner(String owner) {
        return dao.getAllByOwner(owner);
    }

    public List<LockedAccount> getAllLargerThan(double amount) {
        return dao.getAllLargerThan(amount);
    }

    public int getNumberOfLockedAccounts() {
        return dao.getNumberOfLockedAccounts();
    }
}
