The users I already added:
qwe => 3 products
asd, zxc => 2 products
kaya, vbn, muhammet => 1 product
fgh, rty, ceng, odtü => No product

There are 4 html files in my project.

index.html: In index page, you can add new user or delete an exist user by providing username for the 
user. For add user and delete user operations, username can not be empty. If you try to add without providing 
username it will give an alert about it. For delete user option, you can not delete an non-exist user.
Also, you can login providing username. If given username does not exist, it will be alert that
says user does not exist. If the user exists, then it will redirects you to user.html page.

user.html: This page is specific to every user. At the top of the page, in User part, you can see username,
user wallet amount and rating for the user. Rating is stored as an array which stores the rates from their sales.
Rating section shows the average of the elements in the array. If the user is not rated yet, it says 'Not rated'.
Below these, there are 2 links. One is Products Page. By clicking this, you can go products.html page to
buy some products. Another is Order History. By clicking this, you can see your order history and also rate your
purchased products. 
        Below that there is a section for operations. First, deposit and withdraw. You can deposit or 
withdraw form your wallet amount of given quantity. Quantity field must be filled. Second is for adding new
product to store. You must provide product name, product price, quantity and photo(as URL). These fields can
not be empty, too. When you click add new product buttoni it will add new product for your store and refresh
page to show added product in store section below the page. Third is drop product section. You can drop
product by providing product name.
        At the bottom of the page, there is store section. You can see your products which are on sale with
its photo, name, price, and quantity. When somebody buys your product, quantity will decrease, and also,
your wallet amonut will increase.

orderhistory.html: In order history page, you can see your orders and you can rate them. Orders are showed
with product name, price, quantity, sellers name and your rating if you rate else rate button to rate the 
product. When you rate products, your rating is added to rating array in the seller object, and this changing
sellers average rating.

product.html: In this page, you can buy products. There is a list of products every user provides. You can
see products name, price, quantity, sellers name, and sellers rating. Below products there is a box for
the quantity you want to buy. You can fill more than one box. The number can not increase total quantity.
When you try to put more than total quantity, it will put max quantity automatically. There is a buy button
on the top of the page. When you click it, it will check total cost and your wallet. If your money is
not enough, it will alert, else order will be completed. Then, you can see your order from order history
page and rate your order. Note that user can not see their products in this page.

This is an example of how I stored users on the cluster(I deleted product photo values for easy understanding):

{
"_id":"5cb9b219dab1567524b12d45",
"owner_id":"5cb744d3e119ca0e913617dd",
"username":"qwe",
"wallet":"50",
"rating":[3,1,2,1,1,1],
"store":[
        {"productname":"keyboard","photo":".jpg","price":30,"quantity":4},
        {"productname":"computer","photo":".jpg","price":50,"quantity":3}
        ],
"orderhistory":[
                {"productname":"kulaklık","photo":".jpg","price":10,"quantity":"1","sellers_name":"zxc","given_rating":4},
                {"productname":"kulaklık","photo":".jpg","price":10,"quantity":"2","sellers_name":"zxc","given_rating":0}
                ]
}