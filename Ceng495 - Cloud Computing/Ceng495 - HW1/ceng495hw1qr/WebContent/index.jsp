<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
    pageEncoding="ISO-8859-1"%>
<!DOCTYPE html>
<html>
	<head>
		<title>QR generator/reader</title>
	</head>
	
	<body>
		<h2>QR Generator</h2>
		
		<form>
			<Label for="data">Name</Label>
			<input type="text" id="data" name="data" required/>
			
			<br><br>
			
			<Label for="color">Code Color</Label>
			<select name="color" id="color">
				<option selected="selected" value="000">Black</option>
				<option value="fff">White</option>
				<option value="f00">Red</option>
				<option value="0f0">Green</option>
				<option value="00f">Blue</option>
			</select>
			
			<br><br>
			
			<Label for="bgcolor">Background Color</Label>
			<select name="bgcolor" id="bgcolor">
				<option selected="selected" value="fff">White</option>
				<option value="000">Black</option>
				<option value="f00">Red</option>
				<option value="0f0">Green</option>
				<option value="00f">Blue</option>
			</select>
			
			<br><br>
			
			<input type="submit" value="Create"/>
			
		</form>
		
		<br><br>
		

		<%	
			String source = "";
					
			if(request.getParameter("data") != null){
				source = "http://api.qrserver.com/v1/create-qr-code/?";
				source = source + request.getQueryString();
			}
		%>

		<img src="<%= source %>">
		
		<h2>QR Reader</h2>
		
			<form enctype="multipart/form-data" action="http://api.qrserver.com/v1/read-qr-code/" method="POST">
				<input type="hidden" name="MAX_FILE_SIZE" value="1048576" />
				
				Choose QR code image to read/scan: 
				
				<br><br>
				
				<input name="file" type="file" />
				
				<input type="submit" value="Read QR code" />
			</form>
	<body>
</html>