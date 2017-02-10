<?php

namespace auth;
use Nette\Security as NS;



class MyAuthenticator extends \Nette\Object implements NS\IAuthenticator
{
    public $database;

    function __construct(\Nette\Database\Context $database)
    {
        $this->database = $database;
    }

    function authenticate(array $credentials)
    {
        list($username, $password) = $credentials;

        $row = $this->database->table('users')
            ->where('email', $username)->fetch();

        if (!$row){
            throw new NS\AuthenticationException('User not found.');
        }

        if (sha1($password) !== $row->heslo){
            throw new NS\AuthenticationException('Invalid password.');
        }

        return new NS\Identity($row->id, $row->role, array('jmeno' => $row->jmeno));
    }
}
