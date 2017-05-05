
for ((  i = 0 ;  i <=10;  i++  ))
do
  echo "Generate key pair $i..."
  openssl genrsa > "10.1.1.$i""_private.pem" 
  openssl rsa -pubout <"10.1.1.$i""_private.pem" >"10.1.1.$i""_public.pem" 
  
done