import React, { useRef } from 'react'

import { hostUsersFilter } from '../../../hooks/useUsers'
import getUserObjects from '../../../common/getUserObjects'

import UserItem from '../../UserItem'
import LayoutBlock from '../../LayoutBlock'

const LIST_ID = 'hosts'
const LIST_TITLE = 'Hosts'
const LIST_COLOR = 'error'

const BlockHosts = ({
  userIds,
  userData,
  updateUserData,
  targetCommentingId
}) => {
  const userActionsRef = useRef()
  const users = getUserObjects(userIds, userData)
  const hostUsers = users.filter(hostUsersFilter).sort()

  userActionsRef.current = {
    default: {
      icon: 'award',
      color: 'primary',
      handler: targetCommentingId
    }
  }

  return (
    <LayoutBlock
      id={LIST_ID}
      color={LIST_COLOR}
      title={LIST_TITLE}
    >
      {hostUsers.map(user => {
        return (
          <UserItem
            {...user}
            key={user.id}
            actionsRef={userActionsRef}
            nameColor={LIST_COLOR}
            updateUserData={updateUserData}
          />
        )
      })}
    </LayoutBlock>
  )
}

export default BlockHosts
