package eu.jpereira.trainings.designpatterns.structural.facade;

import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;
import eu.jpereira.trainings.designpatterns.structural.facade.model.Book;
import eu.jpereira.trainings.designpatterns.structural.facade.model.Customer;
import eu.jpereira.trainings.designpatterns.structural.facade.model.DispatchReceipt;
import eu.jpereira.trainings.designpatterns.structural.facade.model.Order;
import eu.jpereira.trainings.designpatterns.structural.facade.service.BookDBService;
import eu.jpereira.trainings.designpatterns.structural.facade.service.CustomerDBService;
import eu.jpereira.trainings.designpatterns.structural.facade.service.CustomerNotificationService;
import eu.jpereira.trainings.designpatterns.structural.facade.service.OrderingService;
import eu.jpereira.trainings.designpatterns.structural.facade.service.WharehouseService;

public class DefaultBookStoreFacade implements BookstoreFacade, BookDBService, CustomerDBService, CustomerNotificationService,
	OrderingService, WharehouseService {
	
	private BookDBService bookService;
    private OrderingService orderingService;
    private CustomerDBService customerService;
    private WharehouseService warehouseService;
    private CustomerNotificationService customerNotificationService;
    
	Book dummyBook;
	Customer dummyCustomer;
	Order dummyOrder;
	DispatchReceipt dummyDispatchReceipt;
		
	@Override
	public void placeOrder(String customerId, String isbn) {
		dummyBook = bookService.findBookByISBN(isbn);
		dummyCustomer = customerService.findCustomerById(customerId);
		dummyOrder = orderingService.createOrder(dummyCustomer, dummyBook);
		dummyDispatchReceipt = warehouseService.dispatch(dummyOrder);
		customerNotificationService.notifyClient(dummyDispatchReceipt);
	}
	
	@Override
	public void setBookService(BookDBService bookService) {
		this.bookService = bookService;
	}

	@Override
	public void setCustomerService(CustomerDBService customerService) {
		this.customerService = customerService;
	}	
	
	@Override
	public void setCustomerNotificationService(
			CustomerNotificationService customerNotificationService) {
		this.customerNotificationService = customerNotificationService;
	}

	@Override
	public void setOrderingService(OrderingService orderingService) {
		this.orderingService = orderingService;
	}

	@Override
	public void setWarehouseService(WharehouseService warehouseService) {
		this.warehouseService = warehouseService;
	}

	@Override
	public Book findBookByISBN(String isbn) {
		return new Book(isbn);
	}

	@Override
	public Customer findCustomerById(String customerId) {
		return new Customer(customerId);
	}

	@Override
	public void notifyClient(Order order) {
		// TODO Auto-generated method stub
	}

	@Override
	public void notifyClient(DispatchReceipt dispatchReceipt) {
		// TODO Auto-generated method stub
	}

	@Override
	public Order createOrder(Customer customer, Book book) {
		return new Order();
	}

	@Override
	public DispatchReceipt dispatch(Order order) {
		System.out.println("dispatch");
		return new DispatchReceipt();
	}

}
