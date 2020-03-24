package toberge.lockedBank;
import javax.persistence.*;
import java.util.List;

public class LockedAccountDAO {
    private EntityManagerFactory emf; // avoiding thread problems...

    public LockedAccountDAO(EntityManagerFactory emf) {
        this.emf = emf;
    }

    private EntityManager getEM() {
        return emf.createEntityManager();
    }

    private void closeEM(EntityManager em) {
        if (em != null && em.isOpen()) em.close();
    }

    public LockedAccount getOne(String accountNumber) {
        EntityManager em = getEM();
        try {
            return em.find(LockedAccount.class, accountNumber);
        } finally {
            closeEM(em);
        }
    }

    public void addOne(LockedAccount account) {
        EntityManager em = getEM();
        try {
            em.getTransaction().begin();
            em.persist(account);
            em.getTransaction().commit();
        } finally {
            closeEM(em);
        }
    }

    public void updateOne(LockedAccount account) {
        EntityManager em = getEM();
        try {
            em.getTransaction().begin();
            em.merge(account);
            em.getTransaction().commit();
        } finally {
            closeEM(em);
        }
    }

    public void deleteOne(String accountNumber) {
        EntityManager em = getEM();
        try {
            LockedAccount account = em.find(LockedAccount.class, accountNumber);
            em.getTransaction().begin();
            em.remove(account);
            em.getTransaction().commit();
        } finally {
            closeEM(em);
        }
    }

    public List<LockedAccount> getAll() {
        EntityManager em = getEM();
        try {
            Query query = em.createQuery("SELECT OBJECT(a) FROM LockedAccount a");
            return query.getResultList();
        } finally {
            closeEM(em);
        }
    }

    public List<LockedAccount> getAllLargerThan(double amount) {
        EntityManager em = getEM();
        try {
            Query query = em.createQuery("SELECT OBJECT(a) FROM LockedAccount a WHERE a.balance > :amount");
            query.setParameter("amount", amount);
            return query.getResultList();
        } finally {
            closeEM(em);
        }
    }


    public List<LockedAccount> getAllByOwner(String owner) {
        EntityManager em = getEM();
        try {
            Query query = em.createQuery("SELECT OBJECT(a) FROM LockedAccount a WHERE a.owner = :owner");
            query.setParameter("owner", owner);
            return query.getResultList();
        } finally {
            closeEM(em);
        }
    }

    public int getNumberOfLockedAccounts() {
        EntityManager em = getEM();
        try{
            Query q = em.createNamedQuery("findNumberOfLockedAccounts");
            Long count = (Long) q.getSingleResult();
            return count.intValue();
        }finally{
            closeEM(em);
        }
    }
}
