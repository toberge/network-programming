package toberge.lockedBank;

import java.util.List;

public interface LockedAccountFacade {
    LockedAccount getOne(String accountNumber);
    void addOne(LockedAccount account);
    void updateOne(LockedAccount account);
    void deleteOne(String accountNumber);
    List<LockedAccount> getAll();
    List<LockedAccount> getAllLargerThan(double amount);
    List<LockedAccount> getAllByOwner(String owner);
}
